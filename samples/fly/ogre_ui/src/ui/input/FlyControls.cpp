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


#include "FlyControls.hpp"
#include "OgreUiPre.hpp"
#include "logic/action/all.hpp"
#include "logic/schema/LogicSchema.hpp"


using namespace logic;
using namespace se_client;
using namespace se_core;
using namespace se_ogre;


namespace ui {

	FlyControls
	::FlyControls()
		:  InputHandler("FlyControls"), isCameraInited_(false)
		, dirUp_(0), dirDown_(0), dirLeft_(0), dirRight_(0), dirRollLeft_(0), dirRollRight_(0)
		, dirForward_(1.2), dirBackward_(1) {
	}


	FlyControls
	::~FlyControls() {
	}


	bool FlyControls
	::moveCamera(float stepDelta) { 

		// Don't try to move the camera if the camera is not in an area
		if(!ClientSchema::camera->pos().hasArea())
			return true;

		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		static ViewPoint camera;
		ClientSchema::camera->worldViewPoint(alpha, camera);
		Quat4 face(camera.face_);

		Ogre::Quaternion f(face.w_, face.x_, face.y_, face.z_);
		O3dSchema::playerCamera->setOrientation(f);
		O3dSchema::playerCamera->setPosition(camera.coor_.x_, camera.coor_.y_, camera.coor_.z_);

		return true; 
	}

	void FlyControls
	::keyPressed(Ogre::KeyEvent* e) {
		bool doDropStone = false;
		switch(e->getKey()) {
		case Ogre::KC_SPACE:
			doDropStone = true;
			break;

		case Ogre::KC_Z:
			dirForward_ = 3;
			break;

		case Ogre::KC_X:
			dirBackward_ = .3;
			break;

		case Ogre::KC_UP:
		case Ogre::KC_W:
			dirUp_ = 1;
			break;

		case Ogre::KC_DOWN:
		case Ogre::KC_S:
			dirDown_ = 1;
			break;

		case Ogre::KC_LEFT:
		case Ogre::KC_A:
			dirLeft_ = 1;
			break;

		case Ogre::KC_RIGHT:
		case Ogre::KC_D:
			dirRight_ = 1;
			break;

		case Ogre::KC_END:
		case Ogre::KC_Q:
			dirRollLeft_ = 1;
			break;

		case Ogre::KC_PGDOWN:
		case Ogre::KC_E:
			dirRollRight_ = 1;
			break;

		case Ogre::KC_ESCAPE:
			SimSchema::simEngine.setGameOver(true);
			break;

		case Ogre::KC_INSERT:
			{
				static int shot = 0;
				O3dSchema::renderEngine->screenshot(shot++);
			}
			break;
		}

		static Parameter p;
		actionPlayerFly.param(dirLR(), 2 * dirUD(), dirLR() * 0.5, speed(), p);
		ClientSchema::playerX->planAction(CHANNEL_MOVEMENT, actionPlayerFly, &p);

		actionRotateCamera.param(dirRoll(), 0, 0, p);
		ActionComponent::get(*ClientSchema::floatingCamera)->planAction(CHANNEL_MOVEMENT, actionRotateCamera, &p);

		if(doDropStone) ClientSchema::playerX->planAction(CHANNEL_EXTRA, actionDropStone, &p);
	}


	void FlyControls
	::keyReleased(Ogre::KeyEvent* e) {
		switch(e->getKey()) {
		case Ogre::KC_UP:
		case Ogre::KC_W:
			dirUp_ = 0;
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
			dirDown_ = 0;
			break;

		case Ogre::KC_END:
		case Ogre::KC_Q:
			dirRollLeft_ = 0;
			break;

		case Ogre::KC_PGDOWN:
		case Ogre::KC_E:
			dirRollRight_ = 0;
			break;

		case Ogre::KC_SPACE:
			break;

		case Ogre::KC_Z:
			dirForward_ = 1.2;
			break;

		case Ogre::KC_X:
			dirBackward_ = 1;
			break;
		}

		static Parameter p;
		actionPlayerFly.param(dirLR(), 2 * dirUD(), dirLR() * 0.5, speed(), p);
		ClientSchema::playerX->planAction(CHANNEL_MOVEMENT, actionPlayerFly, &p);

		actionRotateCamera.param(dirRoll(), 0, 0, p);
		ActionComponent::get(*ClientSchema::floatingCamera)->planAction(CHANNEL_MOVEMENT, actionRotateCamera, &p);
	}


}
