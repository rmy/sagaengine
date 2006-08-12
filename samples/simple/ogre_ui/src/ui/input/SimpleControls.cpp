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


#include "SimpleControls.hpp"
#include "OgreUiPre.hpp"
#include "logic/action/all.hpp"


using namespace logic;
using namespace se_core;
using namespace se_ogre;


namespace ui {

	SimpleControls
	::SimpleControls()
		:  InputHandler("SimpleControls")
		, dirUp_(0), dirDown_(0), dirLeft_(0), dirRight_(0), dirRollLeft_(0), dirRollRight_(0)
		, dirForward_(0), dirBackward_(0) {
	}


	SimpleControls
	::~SimpleControls() {
	}


	void SimpleControls
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
			break;

		case Ogre::KC_X:
			break;

		case Ogre::KC_UP:
		case Ogre::KC_W:
			dirForward_ = 1;
			break;

		case Ogre::KC_DOWN:
		case Ogre::KC_S:
			dirBackward_ = 0.5;
			break;

		case Ogre::KC_LEFT:
		case Ogre::KC_A:
			dirLeft_ = 1;
			break;

		case Ogre::KC_RIGHT:
		case Ogre::KC_D:
			dirRight_ = 1;
			break;

		case Ogre::KC_Q:
			SimSchema::simEngine.setGameOver(true);
			break;
		}

		static Parameter p;

		actionForward.param(dirForward(), p);
		ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionForward, &p);
		ClientSchema::player->setDefaultMovementAction(actionForward, &p);

		actionRotate.param(dirLR(), 0, 0, p);
		ClientSchema::player->planAction(CHANNEL_DIRECTION, actionRotate, &p);

		//ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionJump);
	}


	void SimpleControls
	::keyReleased(Ogre::KeyEvent* e) {
		switch(e->getKey()) {
		case Ogre::KC_UP:
		case Ogre::KC_W:
			dirForward_ = 0;
			break;

		case Ogre::KC_LEFT:
		case Ogre::KC_A:
			dirLeft_ = 0;
			break;

		case Ogre::KC_RIGHT:
		case Ogre::KC_D:
			dirRight_ = 0;
			break;

		case Ogre::KC_DOWN:
		case Ogre::KC_S:
			dirBackward_ = 0;
			break;
		}

		static Parameter p;

		actionForward.param(dirForward(), p);
		ClientSchema::player->planAction(CHANNEL_MOVEMENT, actionForward, &p);
		//ClientSchema::player->setDefaultMovementAction(actionForward, &p);

		actionRotate.param(dirLR(), 0, 0, p);
		ClientSchema::player->planAction(CHANNEL_DIRECTION, actionRotate, &p);
	}


}
