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


#include "SimpleAreaFactory.hpp"
#include "SimpleArea.hpp"


using namespace se_core;

namespace se_basic {

	SimpleAreaFactory
	::SimpleAreaFactory(String* name, coor_tile_t w, coor_tile_t h, se_core::ByteArray* data)
			: AreaFactory(name), data_(data), width_(w), height_(h) {
	}


	SimpleAreaFactory
	::~SimpleAreaFactory() {
		delete data_;
	}


	Area* SimpleAreaFactory
	::create(String* name, int pageX, int pageY, int pageZ) const {
		SimpleArea* a = new SimpleArea(this, name, width_, height_);
		createComponents(a);
		createGenericComponents(a);

		a->setPage(pageX, pageY, pageZ);
		PosComponent::Ptr aPos(*a);
		aPos->nextPos().localCoor().x_ = CoorT::fromTile(pageX * a->width());
		aPos->nextPos().localCoor().z_ = CoorT::fromTile(pageZ * a->height());
		aPos->nextPos().world_.setViewPoint(aPos->nextPos().local_);
		aPos->flip();

		return a;
	}

}
