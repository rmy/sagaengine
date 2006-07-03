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


#include "ThingEntity.hpp"
#include "all.hpp"
#include "../area/WorldManager.hpp"
#include "../gfx/canvas/all.hpp"
#include "../input/all.hpp"
#include "../schema/NCursSchema.hpp"
#include "util/math/CoorT.hpp"
#include "util/math/Trig.hpp"
#include "util/error/Log.hpp"
#include "sim/thing/Thing.hpp"
#include "pc/schema/PCSchema.hpp"


using namespace se_core;


namespace se_ncurs {
	ThingEntity
	::ThingEntity()
		: icon_(0), thing_(0) {
	}


	ThingEntity
	::ThingEntity(Thing& thing)
		: icon_(0), thing_(&thing) {
		icon_ = thing_->name()[0];
		move(0);
	}


	ThingEntity
	::~ThingEntity() {
	}


	void ThingEntity
	::move(long when) {
	}


	void ThingEntity
	::render() {
		if(thing_) {
			NCursSchema::canvas().drawEnemy(thing_->pos().x_, thing_->pos().z_, thing_->pos().radius(), icon_);
		}
	}

}
