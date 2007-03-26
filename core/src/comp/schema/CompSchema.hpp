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


#ifndef CompSchema_hpp
#define CompSchema_hpp

#include "comp/comp.hpp"
#include "comp/list/comp_list.hpp"
#include "util/template/SinglyLinkedList.hpp"

/**
 * The game simulation engine. This is the core of SagaEngine.
 */
namespace se_core {
	/**
	 * Global objects and methods used by the simulation logic.
	 */
	namespace CompSchema {
		static const int MAX_OBJECTS = 512;
		static const int MAX_GAME_OBJECTS = 16384;
		typedef SinglyLinkedList<void, MAX_GAME_OBJECTS, 211> VoidList;

		extern _SeCoreExport Composite& activeRoot();
		extern _SeCoreExport Composite& inactiveRoot();

		_SeCoreExport ObjectRepository& objectRepository();

		extern _SeCoreExport VoidList voidList;
	}
}

#endif
