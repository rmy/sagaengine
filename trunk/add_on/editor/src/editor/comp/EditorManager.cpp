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
#include "o3d/area/O3dManager.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "sim/schema/SimSchema.hpp"
#include "editor/schema/EditorSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"

using namespace se_core;
using namespace se_editor;
using namespace se_ogre;

namespace se_editor {
	EditorManager
	::EditorManager()
		: RootComponent(se_core::sct_EDITOR), isEditing_(false) {
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
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorAreaComponent& c = static_cast<EditorAreaComponent&>(it.next());
			//c.propagate();
		}
		*/
	}


	void EditorManager
	::initGame() {
		isEditing_ = false;
	}


	void EditorManager
	::cleanupGame() {
	}

	void EditorManager
	::startEditor() {
		isEditing_ = true;
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorAreaComponent& c = static_cast<EditorAreaComponent&>(it.next());
			c.startEditor();
		}
		O3dSchema::worldManager->setEditLights(true);
		EditorSchema::lastSpawn = 0;
	}


	void EditorManager
	::exitEditor() {
		isEditing_ = false;
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorAreaComponent& c = static_cast<EditorAreaComponent&>(it.next());
			c.exitEditor();
		}
		O3dSchema::worldManager->setEditLights(false);
		EditorSchema::lastSpawn = 0;
	}
}
