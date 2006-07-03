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


#include "O3dPre.H"
#include "O3dInputBridge.hpp"
#include "o3d/area/WorldManager.hpp"
#include "o3d/config/o3d_config.hpp"
#include "o3d/input/InputHandler.hpp"
#include "o3d/input/InputManager.hpp"
#include "o3d/input/Console.hpp"
#include "o3d/schema/O3dSchema.hpp"

using namespace se_core;


namespace se_ogre {

	O3dInputBridge
	::O3dInputBridge(Ogre::RenderWindow* win)
			: keyState_(0) {
		eventProcessor_ = new Ogre::EventProcessor();
		eventProcessor_->initialise(win);
		eventProcessor_->startProcessingEvents();
		eventProcessor_->addKeyListener(this);
		eventProcessor_->addMouseListener(this);
		eventProcessor_->addMouseMotionListener(this);
	}


	O3dInputBridge
	::~O3dInputBridge() {
		delete eventProcessor_;
	}


	void O3dInputBridge
	::keyPressed(Ogre::KeyEvent* e) {
		// Tab flips focus on console
		if(e->getKey() == Ogre::KC_TAB) {
			if(O3dSchema::console) {
				O3dSchema::console->flipFocus();
			}
		}

		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->keyPressed(e);
		}
		// If not, send to game input handler
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->keyPressed(e);
		}


		switch(e->getKey()) {
		case Ogre::KC_ESCAPE:
			SimSchema::simEngine.setGameOver(true);
			break;
		case Ogre::KC_1:
			O3dSchema::playerCamera->setPolygonMode(Ogre::PM_WIREFRAME);
			break;
		case Ogre::KC_2:
			O3dSchema::worldManager->showDebugOverlay(true);
			break;
		case Ogre::KC_0:
			O3dSchema::worldManager->showDebugOverlay(false);
			O3dSchema::playerCamera->setPolygonMode(Ogre::PM_SOLID);
			break;
		}
	}


	void O3dInputBridge
	::keyReleased(Ogre::KeyEvent* e) {
		if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->keyReleased(e);
		}
	}


	void O3dInputBridge
	::mousePressed(Ogre::MouseEvent* e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mousePressed(e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mousePressed(e);
		}
	}


	void O3dInputBridge
	::mouseReleased(Ogre::MouseEvent* e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseReleased(e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseReleased(e);
		}
	}

	void O3dInputBridge
	::mouseMoved(Ogre::MouseEvent* e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseMoved(e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseMoved(e);
		}
	}


	void O3dInputBridge
	::mouseDragged(Ogre::MouseEvent* e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseDragged(e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseDragged(e);
		}
	}

}
