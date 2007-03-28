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
#include "sim/pos/PosComponent.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "sim/script/ScriptComponent.hpp"

using namespace se_core;

namespace se_editor {
	EditorComponent
	::EditorComponent(Composite* owner, const ComponentFactory* factory)
			: AreaChildComponent(sct_EDITOR, owner), string_(0)
			, isEditing_(false) {
		PosComponent::Ptr pos(*this);
		Assert(!pos.isNull());
		/*
		isGrounded_ = pos->nextPos().isGrounded();
		EditorAreaComponent::Ptr editorArea(pos->nextPos().area());
		Assert(!editorArea.isNull());
		startArea_ = editorArea;
		start_.setViewPoint(pos->nextPos().local_);
		string_ = editorArea->grabString();
		*/
	}


	EditorComponent
	::~EditorComponent() {
	}


	void EditorComponent
	::setActive(bool state) {
		if(state)
			init();
		else
			cleanup();
	}


	void EditorComponent
	::init() {
		if(EditorManager::singleton().isEditing()) {
			if(!isEditing_) {
				PosComponent::Ptr pos(this);
				Assert(!pos.isNull());
				isGrounded_ = pos->nextPos().isGrounded();
				EditorAreaComponent::Ptr editorArea(pos->nextPos().area());
				Assert(!editorArea.isNull());
				startArea_ = const_cast<PosComponent*>(pos->nextPos().area());
				Assert(startArea_ != 0);
				start_.setViewPoint(pos->nextPos().local_);
				//string_ = editorArea->grabString();

				PhysicsComponent::Ptr pPhysics(*this);
				if(!pPhysics.isNull()) {
					pPhysics->clearPhysics();
					pPhysics->pushPhysics("FollowMouse");
				}

				ScriptComponent::Ptr pScript(*this);
				pScript->clearScripts();

				isEditing_ = true;
			}
		}
	}


	void EditorComponent
	::cleanup() {
		if(isEditing_ && !isDead()) {
			//PhysicsComponent::Ptr pPhysics(this);
			//if(!pPhysics.isNull())
			//	pPhysics->popPhysics();
			isEditing_ = false;
		}
	}


	void EditorComponent
	::setStart(const Pos& p) {
		/*
		p.setArea(*PosComponent::Ptr(*startArea_));
		p.local_.setViewPoint(start_);
		p.updateWorldViewPoint();
		*/
		startArea_ = const_cast<PosComponent*>(p.area());
		p.areaViewPoint(*p.area(), start_);
	}

}
