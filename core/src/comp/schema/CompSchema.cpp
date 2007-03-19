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


#include "CompSchema.hpp"
#include "../Composite.hpp"
//#include "../ComponentManager.hpp"
//#include "../list/VoidList.hpp"
//#include "../stat/SortedSimObjectList.hpp"


namespace se_core {
	namespace CompSchema {
		/*
		extern _SeCoreExport Composite& activeRoot() {
			return ComponentManager::activeRoot();
		}

		extern _SeCoreExport Composite& inactiveRoot() {
			return ComponentManager::inactiveRoot();
		}

		SortedSimObjectList& sortedSimObjectList() {
			static SortedSimObjectList sgol;
			return sgol;
		}
		*/

		VoidList voidList(__FILE__);
	}
}
