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


#include "O3dPre.hpp"
#include "O3dInputBridge.hpp"
#include "o3d/area/O3dManager.hpp"
#include "o3d/config/o3d_config.hpp"
#include "o3d/input/InputHandler.hpp"
#include "o3d/input/InputManager.hpp"
#include "o3d/input/Console.hpp"
#include "o3d/schema/O3dSchema.hpp"

using namespace se_core;


namespace se_ogre {

	O3dInputBridge
	::O3dInputBridge(Ogre::RenderWindow* win)
			: oldJoyButtons_(0), keyState_(0) {

		eventProcessor_ = new Ogre::EventProcessor();
		eventProcessor_->initialise(win);
		eventProcessor_->startProcessingEvents();
		eventProcessor_->addKeyListener(this);
		eventProcessor_->addMouseListener(this);
		eventProcessor_->addMouseMotionListener(this);

		// BEGIN INPUT INITIALIZATION
		bool bufferedKeys = true;
		bool bufferedMouse = true;
		bool bufferedJoy = true;

		size_t windowHnd = 0;
#if defined OIS_WIN32_PLATFORM
		win->getCustomAttribute("HWND", &windowHnd);
#elif defined OIS_LINUX_PLATFORM
		win->getCustomAttribute("GLXWINDOW", &windowHnd);
#endif
		//Both x11 and Win32 use handle of some sort..
		std::ostringstream windowHndStr;
		windowHndStr << windowHnd;

		OIS::ParamList pl;	
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		OIS::InputManager &im = *OIS::InputManager::createInputSystem( pl );
		//keyboard_ = static_cast<OIS::Keyboard*>(im.createInputObject( OIS::OISKeyboard, bufferedKeys ));
		//mouse_ = static_cast<OIS::Mouse*>(im.createInputObject( OIS::OISMouse, bufferedMouse ));
		try
		{
			joy_ = static_cast<OIS::JoyStick*>(im.createInputObject( OIS::OISJoyStick, bufferedJoy ));
			joy_->setEventCallback(this);
		}
		catch(OIS::Exception& e)
		{
			LogWarning(e.eText);
			joy_ = 0;
		}
	}


	O3dInputBridge
	::~O3dInputBridge() {
		delete eventProcessor_;
	}


	void O3dInputBridge
	::step() {
		if(joy_) {
			joy_->capture();

			int buttonCount = joy_->buttons();
			int buttons = joy_->getJoyStickState().buttons;
			for(int i = 0; i < buttons; ++i) {
				int mask = (1L << i);
				if((buttons & mask) != (oldJoyButtons_ & mask)) {
					if((buttons & mask) == 0) {
						O3dSchema::inputManager().active()->joyButtonPressed(i);
					}
					else {
						O3dSchema::inputManager().active()->joyButtonReleased(i);
					}
				}
			}

			oldJoyButtons_ = buttons;
		}
	}


	void O3dInputBridge
	::keyPressed(Ogre::KeyEvent* e) {
		// Tab flips focus on console
		if(e->getKey() == Ogre::KC_TAB) {
			if(O3dSchema::console) {
				O3dSchema::console->flipFocus();
			}
		}

		bool isProcessed = false;
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->keyPressed(e);
			isProcessed = true;
		}
		// If not, send to game input handler
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->keyPressed(e);
			isProcessed = true;
		}

		switch(e->getKey()) {
		case Ogre::KC_ESCAPE:
			SimSchema::simEngine.setGameOver(true);
			break;
		case Ogre::KC_SCROLL:
			O3dSchema::worldManager->flipDebugOverlay();
			break;
		case Ogre::KC_PAUSE:
			{
				Ogre::PolygonMode mode = O3dSchema::playerCamera->getPolygonMode();
				switch(mode) {
				case Ogre::PM_SOLID:
					mode = Ogre::PM_WIREFRAME;
					break;
				case Ogre::PM_WIREFRAME:
					mode = Ogre::PM_SOLID;
					break;
				}
				O3dSchema::playerCamera->setPolygonMode(mode);
			}
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


	bool O3dInputBridge
	::buttonPressed (const OIS::JoyStickEvent &arg, int button) {
		return O3dSchema::inputManager().active()->buttonPressed(arg, button);
	}

	bool O3dInputBridge
	::buttonReleased (const OIS::JoyStickEvent &arg, int button) {
		return O3dSchema::inputManager().active()->buttonReleased(arg, button);
	}

	bool O3dInputBridge
	::axisMoved (const OIS::JoyStickEvent &arg, int axis) {
		return O3dSchema::inputManager().active()->axisMoved(arg, axis);
	}

}
