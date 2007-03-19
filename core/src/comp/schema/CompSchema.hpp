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

#include "../comp.hpp"
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
		static const int MAX_GAME_OBJECTS = 16384;
		typedef SinglyLinkedList<void, MAX_GAME_OBJECTS, 211> VoidList;
		/*
		class _SeCoreExport VoidList : public VL {
		public:
			VoidList() : VL(__FILE__) {}
		};
		*/


		//extern _SeCoreExport SimComposite& activeRoot();
		//extern _SeCoreExport SimComposite& inactiveRoot();

		/**
		 * List of named instances of SimObject. 
		 * If you need a script or physics object by name instead of by object reference, this
		 * is where you can query for them. This is an important resource for file parsers
		 * and factory classes.
		 */
		//_SeCoreExport SortedSimObjectList& sortedSimObjectList();

		extern _SeCoreExport VoidList voidList;
	}
}

#endif
