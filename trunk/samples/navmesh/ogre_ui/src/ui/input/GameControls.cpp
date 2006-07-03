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


#include "GameControls.hpp"
#include "OgreUiPre.H"
#include "game/action/all.hpp"


using namespace game;
using namespace se_core;
using namespace se_ogre;


namespace ui {

	GameControls
	::GameControls()
			:  InputHandler("GameControls") {
	}


	GameControls
	::~GameControls() {
	}


	void GameControls
	::keyPressed(Ogre::KeyEvent* e) {
		if(ClientSchema::player->hasActiveScript()) {
			if(O3dSchema::speechBubble)
				O3dSchema::speechBubble->trackUserFeedback();
			return;
		}

		switch(e->getKey()) {
		case Ogre::KC_SPACE:
			if(O3dSchema::speechBubble)
				O3dSchema::speechBubble->trackUserFeedback();
			break;

		case Ogre::KC_Z:
			ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionJump);
			break;

		case Ogre::KC_X:
			break;

		case Ogre::KC_UP:
		case Ogre::KC_W:
			ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionForward);
			ClientSchema::player->setDefaultMovementAction(actionForward);
			break;

		case Ogre::KC_DOWN:
		case Ogre::KC_S:
			ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionBackward);
			ClientSchema::player->setDefaultMovementAction(actionBackward);
			break;

		case Ogre::KC_LEFT:
		case Ogre::KC_A:
			ClientSchema::player->planAction(CHANNEL_DIRECTION, actionRotateLeft);
			break;

		case Ogre::KC_RIGHT:
		case Ogre::KC_D:
			ClientSchema::player->planAction(CHANNEL_DIRECTION, actionRotateRight);
			break;

		case Ogre::KC_Q:
			SimSchema::simEngine.setGameOver(true);
			break;
		}
	}


	void GameControls
	::keyReleased(Ogre::KeyEvent* e) {
		switch(e->getKey()) {
		case Ogre::KC_UP:
		case Ogre::KC_W:
			ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionStop);
			ClientSchema::player->setDefaultMovementAction(actionStop);
			break;

		case Ogre::KC_LEFT:
		case Ogre::KC_A:
		case Ogre::KC_RIGHT:
		case Ogre::KC_D:
			ClientSchema::player->planAction(CHANNEL_DIRECTION, actionRotateNone);
			break;

		case Ogre::KC_DOWN:
		case Ogre::KC_S:
			ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionStop);
			ClientSchema::player->setDefaultMovementAction(actionStop);
			break;

		}
	}


}
