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


#include "EditorControls.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "logic/action/all.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/Idle.hpp"
#include "sim/SimEngine.hpp"
#include "sim/physics/PhysicsComponent.hpp"
#include "../action/AcMoveCursor.hpp"
#include "io/schema/IoSchema.hpp"
#include "io/stream/FileManager.hpp"

#include "logic/action/LevelDesignSnap.hpp"
#include "logic/action/LevelDesignSpawn.hpp"
#include "logic/action/LevelDesignCleanRoom.hpp"

#include <OgreInput.h>
#include <OgreKeyEvent.h>

using namespace se_core;
using namespace logic;

namespace se_editor {
	EditorControls& EditorControls
	::singleton() {
		static EditorControls ec;
		return ec;
	}

	EditorControls
	::EditorControls()
			: InputHandler("EditorControls"), Script("EditorControls") {
		init();
	}


	EditorControls
	::~EditorControls() {
	}


	void EditorControls
	::init() {
		cursor_.setIdentity();
		Parameter tmp;
		actionMoveCursor.param(cursor_, tmp);
		defaultAction_[ CHANNEL_MOVEMENT ].setAction(actionMoveCursor);
		defaultAction_[ CHANNEL_MOVEMENT ].copyParameter(tmp);
		defaultAction_[ CHANNEL_DIRECTION ].setAction(actionIdle);
		defaultAction_[ CHANNEL_EXTRA ].setAction(actionIdle);

		plannedAction_[ CHANNEL_MOVEMENT ].resetAction();
		plannedAction_[ CHANNEL_DIRECTION ].resetAction();
		plannedAction_[ CHANNEL_EXTRA ].resetAction();
	}


	void EditorControls
	::nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, ActionAndParameter& out) const {
		if(plannedAction_[ channel ].hasAction()) {
			out.set(plannedAction_[ channel ]);
			plannedAction_[ channel ].resetAction();
			return;
		}
		out.set(defaultAction_[channel]);
	}


	void EditorControls
	::setAction(int channel, const Action& a, const Parameter* p, int deadline) {
		plannedAction_[ channel ].setAction(a);
		if(p) plannedAction_[ channel ].copyParameter(*p);
	}


	void EditorControls
	::setDefaultAction(int channel, const Action& a, const Parameter* p) {
		defaultAction_[ channel ].setAction(a);
		if(p) defaultAction_[ channel ].copyParameter(*p);
	}


	void EditorControls
	::mouseDragged(Ogre::MouseEvent* e) {
		mouseMoved(e);
	}


	void EditorControls
	::mouseMoved(Ogre::MouseEvent* e) {
		cursor_.coor_.x_ += e->getRelX();
		cursor_.coor_.z_ += e->getRelY();
		//cursor_.coor_.y_ += e->getRelZ();
		cursor_.face_.yaw_ += e->getRelZ() * 256;
		LogWarning(cursor_);
	}



	void EditorControls
	::mousePressed(Ogre::MouseEvent* e) {
		switch(e->getButtonID()) {

		case Ogre::MouseEvent::BUTTON0_MASK:
			break;


		case Ogre::MouseEvent::BUTTON1_MASK:
			cursor_.setIdentity();
			break;
		}
		e->consume();
	}


	void EditorControls
	::keyPressed(Ogre::KeyEvent* e) {
		switch(e->getKey()) {
		case Ogre::KC_COMMA:
			setAction(CHANNEL_EXTRA, actionLevelDesignSnap);
			break;

		case Ogre::KC_F1:
			spawnCreature(0, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F2:
			spawnCreature(1, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F3:
			spawnCreature(2, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F4:
			spawnCreature(3, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F5:
			spawnCreature(4, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F6:
			spawnCreature(5, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F7:
			spawnCreature(6, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F8:
			spawnCreature(7, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F9:
			spawnCreature(8, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F10:
			spawnCreature(9, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F11:
			spawnCreature(-2, e->isShiftDown(), e->isAltDown());
			break;
		case Ogre::KC_F12:
			spawnCreature(-1, e->isShiftDown(), e->isAltDown());
			break;

		case Ogre::KC_P:
			{
				char buffer[256];
				sprintf(buffer, "save/%s.txt", SimSchema::simEngine.level());
				IoSchema::fileManager->save(buffer);
			}
			break;

		case Ogre::KC_O:
			setAction(CHANNEL_EXTRA, actionLevelDesignCleanRoom);
			break;
		}
	}


	void EditorControls
	::spawnCreature(int id, bool mod1, bool mod2) {
		int index = id;
		if(index >= 0) {
			if(mod1)
				index += 10;
			if(mod2)
				index += 20;
		}
		Parameter tmp;
		actionLevelDesignSpawn.param(index, tmp);
		setAction(CHANNEL_EXTRA, actionLevelDesignSpawn, &tmp);
		//ClientSchema::playerX->planAction(CHANNEL_EXTRA, actionLevelDesignSpawn, &tmp);
	}
}
