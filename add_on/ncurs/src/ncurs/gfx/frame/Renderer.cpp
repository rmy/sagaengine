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


#include "Renderer.hpp"
#include "../canvas/Canvas.hpp"
#include "../../schema/NCursSchema.hpp"
#include "../../thing/all.hpp"
#include "sim/SimListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_ncurs {
using namespace se_core;


	Renderer
	::Renderer() {
		SimSchema::engineListeners().addListener(*this);
	}


	Renderer
	::~Renderer() {
		SimSchema::engineListeners().removeListener(*this);
	}


	void Renderer
	::preSimTickEvent(long when) {
	}


	void Renderer
	::postSimTickEvent(long when) {
		render(when);
	}


	void Renderer
	::render(long when) {
		NCursSchema::canvas().renderBackground();
		ThingEntityList::iterator_type it = NCursSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = NCursSchema::thingEntityList.next(it);
			te->move(when);
			te->render();
		}
		NCursSchema::canvas().flip();
	}

}
