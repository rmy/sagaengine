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


#include "Action.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"


namespace se_core {

	Action
	::Action(const char* name) : SimObject(got_ACTION, name) {
		// Don't register in sorted list here. It will disable us from
		// using const objects in statically linked libraries.
		//
		// Also not needed unless you implement some scripting
		// that are working using the sorted list
		// alone. This is not ideal, as actions often need some
		// specific parameter tweaking.
		//
		// If you want to do this, register the wanted actions
		// explicitly in an init fucntion by doing the following call:
		//
		//    registerInSortedList();
		//
	}



}
