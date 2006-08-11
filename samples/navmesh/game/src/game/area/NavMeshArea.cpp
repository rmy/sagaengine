#include "GamePre.H"
#include "NavMeshArea.hpp"
#include "NavMesh.hpp"

using namespace se_core;

namespace game {
	NavMeshArea
	::NavMeshArea(String* name, coor_tile_t w, coor_tile_t h, const NavMesh* navMesh)
			: Area (name, w, h), navMesh_(navMesh) {
	}


	NavMeshArea
	::~NavMeshArea() {
	}


	coor_t NavMeshArea
	::groundHeight(const Coor& coor, short index) const {
		static Tuple3 b;
		Point2 p(coor.x_, coor.z_);
		short tri = (index >= 0) ? index : navMesh_->find(p);
		navMesh_->barycentric(tri, p, b);
		return navMesh_->height(tri, b);
	}


	short NavMeshArea
	::index(const se_core::Coor& worldCoor, short oldIndex) const {
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
	::force(const Coor& coor, Force& dest) const {
		// No up or down in NavMeshArea (yet)
		dest.set(0, 0, 0);
	}


	short NavMeshArea
	::terrainStyle(const Coor& coor, short index) const {
		// Only one rule - don't walk outside the area.
		coor_tile_t x = coor.xTile();
		coor_tile_t y = coor.zTile();

		//if(!isLegalCoor(x, y)) {
		//	return TS_VOID;
		//}

		//Point2(coor.x_, coor.z_);
		short tri = (index >= 0) ? index : navMesh_->find(Point2(coor.x_, coor.z_));
		//short tri = navMesh_->find(p);
		//LogMsg(tri << ": " << coor.x_ << ", " << coor.z_);	

		if(tri < 0) {
			return TS_VOID;
		}

		return TS_WALKABLE;
	}


	short NavMeshArea
	::nextTerrainStyle(bray_t direction, const Coor& coor) {
		// May be deprecated soon??
		LogFatal("Not implemented");
		return 0;
	}


	bool NavMeshArea
	::isLineOfSight(const Pos& from, const Pos& to) {
		navMesh_->isInLineOfSight(from, to);
	}


	void NavMeshArea
	::path(const Pos& from, const Pos& to, Point3& out) const {
		// Already in same triangle
		if(from.index() == to.index()) {
			out.set(to.localCoor());
			return;
		}
		if(navMesh_->isInLineOfSight(from, to)) {
			out.set(to.localCoor());
			return;
		}
		short via = navMesh_->path(from.index(), to.index());
		//LogMsg("D:" << from.index() << ", " << to.index() << ": " << via);
		navMesh_->center(via, out);
	}

}