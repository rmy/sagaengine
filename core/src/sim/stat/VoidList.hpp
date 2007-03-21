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


#ifndef VoidList_hpp
#define VoidList_hpp

#include "../sim.hpp"
#include "util/template/SinglyLinkedList.hpp"
#include "sim/config/all.hpp"
#include "../thing/sim_thing.hpp"
#include "../script/sim_script.hpp"


namespace se_core {
	typedef SinglyLinkedList<void, MAX_GAME_OBJECTS, 2> VL;
	class _SeCoreExport VoidList : public VL {
	public:
		VoidList() : VL(__FILE__) {}
		inline Component* nextSimComponent(iterator_type& iterator) {
			return reinterpret_cast<Component*>(next(iterator));
		}
		inline SimObject* nextSimObject(iterator_type& iterator) {
			return reinterpret_cast<SimObject*>(next(iterator));
		}
	};
}

#endif
