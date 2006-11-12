#include "BasicPre.hpp"
#include "NavMeshArea.hpp"
#include "NavMesh.hpp"

using namespace se_core;

namespace se_basic {
	NavMeshArea
	::NavMeshArea(String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh)
			: Area (name, w, h), navMesh_(navMesh) {
		LogMsg("Creating area " << name << " with size " << w << ", " << h);
	}


	NavMeshArea
	::~NavMeshArea() {
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

		Point2 p(worldCoor.x_ - nextPosition_.world_.coor_.x_
				 , worldCoor.z_ - nextPosition_.world_.coor_.z_);

		//LogMsg(p.x_ << ", " << p.y_);

		short index;
		if(oldIndex >= 0) {
			index = navMesh_->find(oldIndex, p);
		}
		else {
			index = navMesh_->find(p);
		}

		//LogMsg(index);
		return index;
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
		//LogMsg(tri << ": " << coor.x_ << ", " << coor.z_);

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
		return navMesh_->isInLineOfSight(from, to);
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
			//LogMsg(name() << p << toArea << " - " << out);

			if(toIndex < 0) {
				out.set(to.localCoor());
				return;
			}

			// Already in same triangle
			if(from.index() == toIndex) {
				// Out is already set
				return;
			}
			short via = navMesh_->path(from.index(), toIndex);
			navMesh_->center(via, out);

			return;
		}
		// Already in same triangle
		if(from.index() == to.index()) {
			out.set(to.localCoor());
			return;
		}
		// In line of sight
		if(navMesh_->isInLineOfSight(from, to)) {
			out.set(to.localCoor());
			return;
		}
		short via = navMesh_->path(from.index(), to.index());
		//LogMsg("D:" << from.index() << ", " << to.index() << ": " << via);
		navMesh_->center(via, out);
	}

}
