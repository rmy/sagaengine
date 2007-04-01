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
#include "../action/AcExitEditor.hpp"
#include "io/schema/IoSchema.hpp"
#include "io/stream/FileManager.hpp"

#include "editor/schema/EditorSchema.hpp"
#include "editor/action/all.hpp"

#include <OgreInput.h>
#include <OgreKeyEvent.h>

using namespace se_core;

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
		isRelative_ = true;
		actionMoveCursor.param(isRelative_, cursor_, tmp);
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
		float speed = 1.0f;
		if(e->isShiftDown()) {
			speed = 4;
		}
		if(!e->isControlDown() && !(e->getModifiers() & e->BUTTON2_MASK) != 0 ) {
			cursor_.coor_.x_ += e->getRelX() * speed;
			cursor_.coor_.z_ += e->getRelY() * speed;
			cursor_.face_.yaw_ += e->getRelZ() * 16 * BRAY_RES * speed;
		}
		else {
			cursor_.face_.yaw_ += e->getRelZ() * 6 * BRAY_RES;
			float r = (1 + e->getRelX()) * (1 + e->getRelY()) - 1;
			cursor_.face_.yaw_ += r * 64 * BRAY_RES;
		}

	}


	void EditorControls
	::spawnCreature(int id, bool mod1, bool mod2) {
		if(id < 0) {
			setAction(CHANNEL_EXTRA, actionLoseGrabbed);
			return;
		}

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
	}

	void EditorControls
	::placeEntrance(int id) {
		Parameter tmp;
		actionPlaceEntrance.param(id, tmp);
		setAction(CHANNEL_EXTRA, actionPlaceEntrance, &tmp);
	}

	void EditorControls
	::mousePressed(Ogre::MouseEvent* e) {
		switch(e->getButtonID()) {
		case Ogre::MouseEvent::BUTTON0_MASK:
			if(EditorSchema::lastSpawn) 
				setAction(CHANNEL_EXTRA, actionPlaceGrabbed);
			else
				setAction(CHANNEL_EXTRA, actionGrabNearest);
			cursor_.setIdentity();
			break;

		case Ogre::MouseEvent::BUTTON1_MASK:
			setAction(CHANNEL_EXTRA, actionLoseGrabbed);
			cursor_.setIdentity();
			break;
		}
		e->consume();
	}


	void EditorControls
	::keyPressed(Ogre::KeyEvent* e) {
		//isRelative_ = !e->isShiftDown();

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

		case Ogre::KC_0:
			placeEntrance(0);
			break;
		case Ogre::KC_1:
			placeEntrance(1);
			break;
		case Ogre::KC_2:
			placeEntrance(2);
			break;
		case Ogre::KC_3:
			placeEntrance(3);
			break;
		case Ogre::KC_4:
			placeEntrance(4);
			break;
		case Ogre::KC_5:
			placeEntrance(5);
			break;
		case Ogre::KC_6:
			placeEntrance(6);
			break;
		case Ogre::KC_7:
			placeEntrance(7);
			break;
		case Ogre::KC_8:
			placeEntrance(8);
			break;
		case Ogre::KC_9:
			placeEntrance(9);
			break;

		case Ogre::KC_P:
			{
				char buffer[256];
				sprintf(buffer, "save/%s.txt", SimSchema::simEngine.level());
				IoSchema::fileManager->save(buffer);
			}
			break;

		case Ogre::KC_O:
			{
				setAction(CHANNEL_EXTRA, actionExitEditor);
			}
			break;

		case Ogre::KC_C:
			{
				if(e->isControlDown()) {
					setAction(CHANNEL_EXTRA, actionCopyLevel);
				}
			}
			break;

		case Ogre::KC_V:
			{
				if(e->isControlDown()) {
					setAction(CHANNEL_EXTRA, actionPasteLevel);
				}
			}
			break;

		case Ogre::KC_BACK:
			setAction(CHANNEL_EXTRA, actionDestroyGrabbed);
			break;

		case Ogre::KC_RETURN:
			setAction(CHANNEL_EXTRA, actionSaveLevel);
			break;

		case Ogre::KC_DELETE:
			if(e->isControlDown()) {
				setAction(CHANNEL_EXTRA, actionLevelDesignCleanRoom);
			}
			else {
				setAction(CHANNEL_EXTRA, actionDestroyGrabbed);
			}
			break;
		}
	}

	void EditorControls
	::keyReleased(Ogre::KeyEvent* e) {
		//isRelative_ = !e->isShiftDown();
		e->consume();
	}
}
