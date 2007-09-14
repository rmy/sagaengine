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
#include "sim/schema/SimSchema.hpp"
#include "sim/message/MessageCentral.hpp"
#include "../action/AcMoveCursor.hpp"
#include "../action/AcExitEditor.hpp"
#include "io/schema/IoSchema.hpp"
#include "io/stream/FileManager.hpp"
#include "editor/schema/EditorSchema.hpp"
#include "editor/action/all.hpp"
#include "sim/stat/Dictionary.hpp"
#include <OIS.h>

using namespace se_core;

namespace se_editor {
	EditorControls& EditorControls
	::singleton() {
		static EditorControls ec;
		return ec;
	}

	EditorControls
	::EditorControls()
			: InputHandler("EditorControls"), Script("EditorControls"), modifier_(100) {
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

		showMenu(modifier(false, false), false);
	}

	void EditorControls
	::lostFocusEvent() {
		LogWarning("Focus lost");
		SimSchema::messageCentral.info(0);
		LogWarning("Focus lost again");
	}

	void EditorControls
	::showMenu(int start, bool mod3) {
		if(mod3) {
			SimSchema::messageCentral.info("F1 - cavern\nF2 - castle\nF3 - courtyard");
			return;
		}
		char buffer[1024];
		char* s = buffer;
		int dictId = SimSchema::dictionary().id(DE_DICTIONARY_TYPE, "LEVEL_DESIGN_SPAWN");
		for(int i = start; i < start + 10; ++i) {
			const char* name = 0;
			if(SimSchema::dictionary().hasId(dictId, i)) {
				//if(SimSchema::dictionary().hasId(dictId + 1, i))
				//	name = SimSchema::dictionary().name(dictId + 1, i);
				//else
				name = SimSchema::dictionary().name(dictId, i);
			}

			if(name) {
				s += sprintf(s, "F%d - %s\n", i - start + 1, name);
			}
			else {
				s += sprintf(s, "F%d\n", i - start + 1);
			}
		}
		SimSchema::messageCentral.info(buffer);
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
	::mouseDragged(const OIS::MouseEvent* e) {
		mouseMoved(e);
	}


	void EditorControls
	::mouseMoved(const OIS::MouseEvent* e) {
		float speed = 1.0f;
		if(isShiftDown()) {
			speed = 4;
		}
		if(!isControlDown() && !e->state.buttonDown(OIS::MB_Middle)) {
			if(EditorSchema::cameraPos == EditorSchema::cam_LEFT) {
				cursor_.coor_.z_ -= e->state.X.rel * (1.0f / 1024.f) * speed;
				cursor_.coor_.x_ += e->state.Y.rel * (1.0f / 1024.f) * speed;
				cursor_.face_.yaw_ += e->state.Z.rel * (1.0f / 1024.f) * 16 * BRAY_RES * speed;
			}
			else {
				cursor_.coor_.x_ += e->state.X.rel * (1.0f / 1024.f) * speed;
				cursor_.coor_.z_ += e->state.Y.rel * (1.0f / 1024.f) * speed;
				cursor_.face_.yaw_ += e->state.Z.rel * (1.0f / 1024.f) * 16 * BRAY_RES * speed;
			}
		}
		else {
			cursor_.face_.yaw_ += e->state.Z.rel * 6 / 1024.f * BRAY_RES;
			float r = (1 + e->state.X.rel) * (1 + e->state.Y.rel) - 1;
			cursor_.face_.yaw_ += r * 64 / 1024.f * BRAY_RES;
		}

	}


	int EditorControls
	::modifier(bool mod1, bool mod2) {
		int m = 0;
		if(mod1)
			m += 10;
		if(mod2)
			m += 20;
		m += modifier_;
		return m;
	}

	void EditorControls
	::spawnCreature(int id, bool mod1, bool mod2, bool mod3) {
		if(id < 0) {
			setAction(CHANNEL_EXTRA, actionLoseGrabbed);
			return;
		}

		if(mod3) {
			modifier_ = id * 100;
			return;
		}

		int index = id;
		index += modifier(mod1, mod2);

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
	::mousePressed(const OIS::MouseEvent* e, int button) {
		switch(button) {
		case OIS::MB_Left:
			if(EditorSchema::lastSpawn) 
				setAction(CHANNEL_EXTRA, actionPlaceGrabbed);
			else
				setAction(CHANNEL_EXTRA, actionGrabNearest);
			cursor_.setIdentity();
			break;

		case OIS::MB_Right:
			setAction(CHANNEL_EXTRA, actionLoseGrabbed);
			cursor_.setIdentity();
			break;
		}
	}


	void EditorControls
	::keyPressed(const OIS::KeyEvent* e) {
		switch(e->key) {
		case OIS::KC_COMMA:
			setAction(CHANNEL_EXTRA, actionLevelDesignSnap);
			break;

		case OIS::KC_F1:
			spawnCreature(0, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F2:
			spawnCreature(1, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F3:
			spawnCreature(2, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F4:
			spawnCreature(3, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F5:
			spawnCreature(4, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F6:
			spawnCreature(5, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F7:
			spawnCreature(6, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F8:
			spawnCreature(7, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F9:
			spawnCreature(8, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F10:
			spawnCreature(9, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F11:
			spawnCreature(-2, isShiftDown(), isAltDown(), isWinDown());
			break;
		case OIS::KC_F12:
			spawnCreature(-1, isShiftDown(), isAltDown(), isWinDown());
			break;

		case OIS::KC_0:
			placeEntrance(0);
			break;
		case OIS::KC_1:
			placeEntrance(1);
			break;
		case OIS::KC_2:
			placeEntrance(2);
			break;
		case OIS::KC_3:
			placeEntrance(3);
			break;
		case OIS::KC_4:
			placeEntrance(4);
			break;
		case OIS::KC_5:
			placeEntrance(5);
			break;
		case OIS::KC_6:
			placeEntrance(6);
			break;
		case OIS::KC_7:
			placeEntrance(7);
			break;
		case OIS::KC_8:
			placeEntrance(8);
			break;
		case OIS::KC_9:
			placeEntrance(9);
			break;

		case OIS::KC_P:
			{
				char buffer[256];
				sprintf(buffer, "save/%s.txt", SimSchema::simEngine.level());
				IoSchema::fileManager->save(buffer);
			}
			break;

		case OIS::KC_O:
			{
				setAction(CHANNEL_EXTRA, actionExitEditor);
			}
			break;

		case OIS::KC_C:
			{
				if(isControlDown()) {
					setAction(CHANNEL_EXTRA, actionCopyLevel);
				}
				else {
					EditorSchema::cameraPos = static_cast<EditorSchema::CameraPos>((EditorSchema::cameraPos + 1) % EditorSchema::cam_MAX);
				}
			}
			break;

		case OIS::KC_LEFT:
			EditorSchema::cameraPos = static_cast<EditorSchema::CameraPos>((EditorSchema::cameraPos - 1 + EditorSchema::cam_MAX) % EditorSchema::cam_MAX);
			break;

		case OIS::KC_RIGHT:
			EditorSchema::cameraPos = static_cast<EditorSchema::CameraPos>((EditorSchema::cameraPos + 1) % EditorSchema::cam_MAX);
			break;

		case OIS::KC_V:
			{
				if(isControlDown()) {
					setAction(CHANNEL_EXTRA, actionPasteLevel);
				}
			}
			break;

		case OIS::KC_BACK:
			setAction(CHANNEL_EXTRA, actionDestroyGrabbed);
			break;

		case OIS::KC_RETURN:
			setAction(CHANNEL_EXTRA, actionSaveLevel);
			break;

		case OIS::KC_DELETE:
			if(isControlDown()) {
				setAction(CHANNEL_EXTRA, actionLevelDesignCleanRoom);
			}
			else {
				setAction(CHANNEL_EXTRA, actionDestroyGrabbed);
			}
			break;

		case OIS::KC_LSHIFT:
		case OIS::KC_RSHIFT:
		case OIS::KC_LWIN:
		case 56: // ALT
			showMenu(modifier(isShiftDown(), isAltDown()), isWinDown());
			break;
		}
	}


	void EditorControls
	::keyReleased(const OIS::KeyEvent* e) {
		switch(e->key) {
		case OIS::KC_LSHIFT:
		case OIS::KC_RSHIFT:
		case OIS::KC_LWIN:
		case 56: // ALT
			showMenu(modifier(isShiftDown(), isAltDown()), isWinDown());
			break;
		}
	}
}
