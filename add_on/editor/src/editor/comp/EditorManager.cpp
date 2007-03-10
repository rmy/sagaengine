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


#include "EditorManager.hpp"
#include "EditorAreaComponent.hpp"
#include "EditorComponent.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/MultiSimNodeComponent.hpp"
#include "util/error/Log.hpp"

using namespace se_editor;

namespace se_editor {
	EditorManager
	::EditorManager()
		: SimComponentManager(se_core::sct_EDITOR) {
	}

	EditorManager
	::~EditorManager() {
	}


	EditorManager&  EditorManager
	::singleton() {
		static EditorManager instance;
		return instance;
	}


	void EditorManager
	::step(long when) {
		/*
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			EditorAreaComponent& c = static_cast<EditorAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void EditorManager
	::initGame() {
	}


	void EditorManager
	::cleanupGame() {
	}

}
