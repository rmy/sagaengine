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
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"
#include "comp/CompositeFactory.hpp"
#include "io/stream/FileManager.hpp"
#include "io/schema/IoSchema.hpp"
#include <cstdio>

using namespace se_core;

namespace se_editor {

	EditorAreaComponent
	::EditorAreaComponent(Composite* owner, const se_core::ComponentFactory* factory) 
			: RootChildComponent(se_core::sct_EDITOR, owner, factory), usedStrings_(0) {
	}


	EditorAreaComponent
	::~EditorAreaComponent() {
	}


	void EditorAreaComponent
	::startEditor() {
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorComponent& c = static_cast<EditorComponent&>(it.next());
			if(c.owner()->component(sct_PLAYER) || c.owner()->component(sct_CAMERA)) {
				continue;
			}
			c.owner()->scheduleForDestruction();
		}
		char buffer[256];
		sprintf(buffer, "logic/area/thing/%s", owner()->name());
		IoSchema::fileManager->loadDirectory(buffer);
	}

	void EditorAreaComponent
	::setActive(bool state) {
		if(state) {
			NodeComponent* c = static_cast<NodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
				if(EditorManager::singleton().isEditing()) {
					startEditor();
				}
			}
		}
		else {
			resetParent();
		}
	}


	/*
	se_core::String* EditorAreaComponent
	::grabString() {
		Assert(usedStrings_ < MAX_STRINGS);
		return &strings_[ usedStrings_++ ];
	}
	*/

}

