/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#include "GamePre.H"
#include "NavMeshArea.hpp"
#include "NavMesh.hpp"

using namespace se_core;

namespace game {
	NavMeshArea
	::NavMeshArea(String* name, coor_tile_t w, coor_tile_t h, ByteArray* data)
			: Area (name, w, h), data_(data) {
		navMesh_ = new const NavMesh(data_->get());
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


	void NavMeshArea
	::updateIndex(se_core::Pos& pos) const {
		short index;
		Point2 p(pos.coor_.x_, pos.coor_.z_);

		if(pos.hasIndex()) {
			index = navMesh_->find(pos.index(), p);
			//LogMsg(index);
		}
		else {
			index = navMesh_->find(p);
			//LogMsg(index);
		}

		if(index >= 0) {
			pos.setIndex(index);
		}
		else {
			pos.setNoIndex();
		}
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
			out.set(to.coor_);
			return;
		}
		if(navMesh_->isInLineOfSight(from, to)) {
			out.set(to.coor_);
			return;
		}
		short via = navMesh_->path(from.index(), to.index());
		//LogMsg("D:" << from.index() << ", " << to.index() << ": " << via);
		navMesh_->center(via, out);
	}

}
