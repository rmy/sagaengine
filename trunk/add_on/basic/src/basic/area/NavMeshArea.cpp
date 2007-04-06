#include "BasicPre.hpp"
#include "NavMeshArea.hpp"
#include "NavMesh.hpp"

using namespace se_core;

namespace se_basic {
	NavMeshArea
	::NavMeshArea(const se_core::CompositeFactory* f, String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh)
			: Area (f, name, w, h), navMesh_(navMesh) {
		LogDetail("Creating area " << name << " with size " << w << ", " << h);
	}


	NavMeshArea
	::~NavMeshArea() {
	}


	bool NavMeshArea
	::isNeighbour(const Area& area) const {
		if(!Area::isNeighbour(area))
			return false;
		
		PosComponent::Ptr aPos(area);
		Point3 p(aPos->pos().worldCoor());
		p.sub(posComponent_->pos().worldCoor());
		BoundingBox toArea(p, aPos->pos().bounds_);

		Point3 tmp;
		short index = navMesh_->findExit(toArea, tmp);
		return (index >= 0);
	}


	coor_t NavMeshArea
	::groundHeight(const Point3& coor, short index) const {
		static Tuple3 b;
		Point2 p(coor.x_, coor.z_);
		short tri = (index >= 0) ? index : navMesh_->find(p);
		Assert(tri >= 0 && "Make sure coor is legal before calling groundHeight");
		navMesh_->barycentric(tri, p, b);
		return navMesh_->height(tri, b);
	}


	short NavMeshArea
	::index(const se_core::Point3& worldCoor, short oldIndex) const {
		//Assert(nextPosition_.face().isIdentity() 
		//	   && "Rotation of area not supported");

		Point2 p(worldCoor.x_ - posComponent_->nextPos().world_.coor_.x_
				 , worldCoor.z_ - posComponent_->nextPos().world_.coor_.z_);

		//LogDetail(p.x_ << ", " << p.y_);

		short index;
		if(oldIndex >= 0) {
			index = navMesh_->find(oldIndex, p);
		}
		else {
			index = navMesh_->find(p);
		}

		//LogDetail(index);
		return index;
	}


	bool NavMeshArea
	::doesTouchVoid(const Point3& wc, short index, coor_t radius) const {
		if(index < 0)
			return true;
		Point3 p(wc);
		p.sub(posComponent_->nextPos().worldCoor());
		return navMesh_->doesTouchVoid(p, index, radius);
	}

	void NavMeshArea
	::force(const Point3& coor, Vector3& dest) const {
		// No up or down in NavMeshArea (yet)
		dest.set(0, 0, 0);
	}


	short NavMeshArea
	::terrainStyle(const Point3& coor, short index) const {
		coor_tile_t x = coor.xTile();
		coor_tile_t y = coor.zTile();

		Point2 p(coor.x_, coor.z_);
		short tri = (index >= 0) ? index : navMesh_->find(p);
		//short tri = navMesh_->find(p);
		//LogDetail(tri << ": " << coor.x_ << ", " << coor.z_);

		if(tri < 0) {
			return TS_VOID;
		}

		return TS_WALKABLE;
	}


	short NavMeshArea
	::nextTerrainStyle(bray_t direction, const Point3& coor) {
		// May be deprecated soon??
		LogFatal("Not implemented");
		return 0;
	}


	bool NavMeshArea
	::isLineOfSight(const Pos& from, const Pos& to) {
		const NavMeshArea* toArea = static_cast<const NavMeshArea*>(to.area()->owner());

		Point3 fromPoint, toPoint;
		short fromIndex, toIndex = -1;

		fromPoint.sub(from.worldCoor(), posComponent_->nextPos().worldCoor());
		fromIndex = from.index();
		toPoint.sub(to.worldCoor(), posComponent_->nextPos().worldCoor());
		if(toArea == this) {
			toIndex = to.index();
		}
		int linkType = navMesh_->isInLineOfSight(fromPoint, fromIndex, toPoint, toIndex);

		// Found correct triangle?
		if(linkType == 0)
			return true;

		// Found wall?
		if(linkType == -1)
			return false;

		// Found link to neighbour area?
		if(linkType == 1) {
			Assert(this != toArea);
			if(!Area::isNeighbour(*toArea))
				return false;

			Point3 offset, tmp;
			PosComponent::Ptr fromAreaPos(*posComponent_);
			PosComponent::Ptr toAreaPos(*toArea->posComponent_);
			offset.sub(toAreaPos->pos().worldCoor(), fromAreaPos->pos().worldCoor());
			BoundingBox toAreaBounds(offset, toAreaPos->pos().bounds_);

			fromPoint.sub(offset);
			fromIndex = toArea->navMesh_->findExit(toAreaBounds, tmp);
			toPoint.sub(offset);
			toIndex = to.index();

			linkType = toArea->navMesh_->isInLineOfSight(fromPoint, fromIndex, toPoint, toIndex);

			// Found correct triangle?
			return (linkType == 0);
		}
		return false;
	}



	coor_t NavMeshArea
	::farthestLineOfSight(const Pos& from, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh) const {
		Point2 p;
		short toIndex, fromIndex;
		Point3 toPoint, fromPoint;

		fromPoint.set(from.worldCoor());
		fromIndex = from.index();

		toPoint.setForward(maxLen, yaw);
		toPoint.y_ = 0;
		toPoint.add(fromPoint);
		toIndex = index(toPoint, -1);


		const NavMeshArea* toArea = static_cast<const NavMeshArea*>(neighbour(toPoint));

		toPoint.sub(posComponent_->nextPos().worldCoor());
		fromPoint.sub(posComponent_->nextPos().worldCoor());

		int linkType = navMesh_->farthestLineOfSightXZ(fromIndex, fromPoint, toPoint, toIndex, p);
		if(linkType == 1 && toArea && toArea != this) {

			Point3 offset, tmp;
			PosComponent::Ptr fromAreaPos(*posComponent_);
			PosComponent::Ptr toAreaPos(*toArea->posComponent_);
			offset.sub(fromAreaPos->pos().worldCoor(), toAreaPos->pos().worldCoor());
			BoundingBox toAreaBounds(offset, toAreaPos->pos().bounds_);

			fromIndex = toArea->navMesh_->findExit(toAreaBounds, tmp);
			fromPoint.x_ = p.x_;
			fromPoint.z_ = p.y_;
			fromPoint.add(offset);

			toPoint.add(posComponent_->nextPos().worldCoor());
			toIndex = toArea->index(toPoint, -1);
			toPoint.sub(toAreaPos->nextPos().worldCoor());

			linkType = toArea->navMesh_->farthestLineOfSightXZ(fromIndex, fromPoint, toPoint, toIndex, p);
			toPoint.x_ = p.x_ + toAreaPos->nextPos().worldCoor().x_;
			toPoint.z_ = p.y_ + toAreaPos->nextPos().worldCoor().z_;
		}
		else {
			toPoint.x_ = p.x_ + posComponent_->nextPos().worldCoor().x_;
			toPoint.z_ = p.y_ + posComponent_->nextPos().worldCoor().z_;
		}

		Vector3 dist;
		dist.sub(from.worldCoor(), toPoint);
		dist.y_ = 0;
		coor_t len = dist.length();
		//len += maxOffNavMesh;
		if(len > maxLen)
			len = maxLen;

		return len;
	}


	coor_t NavMeshArea
	::farthestLineOfSight(const Pos& from, bray_t yaw, coor_t maxLen, coor_t maxOffNavMesh, Point3& dest) const {
		coor_t len = farthestLineOfSight(from, yaw, maxLen, maxOffNavMesh);
		Point3 toPoint;
		toPoint.setForward(len, yaw);
		toPoint.add(from.worldCoor());
		if(toPoint.xzDistanceSquared(from.worldCoor()) < dest.xzDistanceSquared(from.worldCoor())) {
			dest.x_ = toPoint.x_;
			dest.z_ = toPoint.z_;
		}

		return len;
	}


	void NavMeshArea
	::farthestLineOfSight(const Pos& from, const Pos& to, Point3& dest) const {
		Point3 toPoint(to.worldCoor());
		short toIndex = index(toPoint, from.index());
		toPoint.sub(posComponent_->nextPos().worldCoor());

		short fromIndex = from.index();
		Point3 fromPoint;
		fromPoint.sub(from.worldCoor(), posComponent_->nextPos().worldCoor());

		Point2 p;
		navMesh_->farthestLineOfSightXZ(fromIndex, fromPoint, toPoint, toIndex, p);
		dest.x_ = p.x_ + posComponent_->nextPos().worldCoor().x_;
		dest.z_ = p.y_ + posComponent_->nextPos().worldCoor().z_;
	}


	short NavMeshArea
	::farthestLos(const Point3& from, short fromIndex, short toIndex) const {
		Point3 c;
		if(fromIndex == toIndex) {
			return toIndex;
		}

		short via = navMesh_->path(fromIndex, toIndex);
		if(via == toIndex)
			return via;
		navMesh_->center(via, c);
		if(navMesh_->isInLineOfSight(from, fromIndex, c, via) < 0) {
			LogWarning("Gave path that was not in LOS");
			return fromIndex;
		}

		short next = navMesh_->path(via, toIndex);
		navMesh_->center(next, c);
		while(navMesh_->isInLineOfSight(from, fromIndex, c, next) == 0) {
			via = next;
			if(via == toIndex)
				break;

			next = navMesh_->path(via, toIndex);
			navMesh_->center(next, c);
		}
		return via;
	}


	void NavMeshArea
	::path(const Pos& from, const Pos& to, Point3& out) const {
		// In different areas - search for triangles in this navmesh that
		// is inside the other
		if(from.area() != to.area()) {
			if(!from.hasArea() || !to.hasArea()) {
				out.set(from.localCoor());
				return;
			}
			Point3 p(to.area()->pos().worldCoor());
			p.sub(from.area()->pos().worldCoor());
			BoundingBox toArea(p, to.area()->pos().bounds_);
			short toIndex = navMesh_->findExit(toArea, out);
			//LogDetail(name() << p << toArea << " - " << out);

			if(toIndex < 0) {
				out.set(to.localCoor());
				return;
			}

			// Already in same triangle
			if(from.index() == toIndex) {
				// At exit - walk to center of next area
				toArea.center(out);
				return;
			}
			if(navMesh_->isInLineOfSight(from.localCoor(), from.index(), out, toIndex) == 0) {
				// Out is already set
				return;
			}
			//short via = navMesh_->path(from.index(), toIndex);
			short via = farthestLos(from.localCoor(), from.index(), toIndex);
			navMesh_->center(via, out);

			return;
		}
		// Already in same triangle
		if(from.index() == to.index()) {
			out.set(to.localCoor());
			return;
		}
		if(to.index() < 0) {
			out.set(to.localCoor());
			return;
		}
		// In line of sight
		if(navMesh_->isInLineOfSight(from, to) == 0) {
			out.set(to.localCoor());
			return;
		}
		short via = farthestLos(from.localCoor(), from.index(), to.index());
		//short via = navMesh_->path(from.index(), to.index());
		//LogDetail("D:" << from.index() << ", " << to.index() << ": " << via);
		navMesh_->center(via, out);
	}


	bray_t NavMeshArea
	::slideAngle(const se_core::Pos& from, const se_core::Point3& to) const {
		Point3 fromAc, toAc, np(posComponent_->nextPos().worldCoor());
		fromAc.sub(from.worldCoor(), np);
		toAc.sub(to, np);
		bray_t angle = navMesh_->slideAngle(fromAc, from.index(), toAc);
		return angle;
	}


	bray_t NavMeshArea
	::wallAngle(const se_core::Pos& from, const se_core::Point3& to) const {
		Point3 fromAc, toAc;
		fromAc.sub(from.worldCoor(), posComponent_->nextPos().worldCoor());
		toAc.sub(to, posComponent_->nextPos().worldCoor());
		bray_t angle = navMesh_->wallAngle(fromAc, from.index(), toAc);
		return angle;
	}


	coor_double_t NavMeshArea
	::findNearest(const se_core::Point3& wc, se_core::Point3& out) const {
		Point3 p(wc);
		p.sub(posComponent_->nextPos().worldCoor());
		return navMesh_->findNearest(p, out);
	}

}
