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


#include "NavMeshAreaFactory.hpp"
#include "NavMeshArea.hpp"
#include "NavMesh.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include "util/type/ByteArray.hpp"

using namespace se_core;

namespace se_basic {

	NavMeshAreaFactory
	::NavMeshAreaFactory(String* name, coor_tile_t w, coor_tile_t h, se_core::ByteArray* data)
			: AreaFactory(name), data_(data), width_(w), height_(h) {
		navMesh_ = new const NavMesh(data->get());
	}


	NavMeshAreaFactory
	::~NavMeshAreaFactory() {
		delete navMesh_;
		delete data_;
	}


	Composite* NavMeshAreaFactory
	::create(String* name, int pageX, int pageY, int pageZ, int gridId) const {
		Composite* c = new Composite(this, name->copyValue());
		NavMeshArea* a = new NavMeshArea(c, 0, width_, height_, navMesh_);
		createComponents(c);
		createGenericComponents(c);

		ZoneAreaComponent::Ptr aZone(*c);
		aZone->page().set(pageX, pageY, pageZ, gridId);
		PosComponent::Ptr aPos(*c);
		aPos->nextPos().localCoor().x_ = CoorT::fromTile(pageX * a->width());
		aPos->nextPos().localCoor().z_ = CoorT::fromTile(pageZ * a->height());
		aPos->nextPos().world_.setViewPoint(aPos->nextPos().local_);
		aPos->flip();

		return c;
	}

	void NavMeshAreaFactory
	::release(Composite* c) const {
		//LogWarning(c->name());
		delete c->component(se_core::sct_BLOB);
		delete c;
	}
}
