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
#include "sim/SimEngine.hpp"
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

using namespace se_core;


namespace se_ogre {

	O3dInputBridge
	::O3dInputBridge(Ogre::RenderWindow* win)
			: oldJoyButtons_(0), keyState_(0), isLeftShiftDown_(false), isRightShiftDown_(false) {

		// BEGIN INPUT INITIALIZATION
		bool bufferedKeys = true;
		bool bufferedMouse = true;
		bool bufferedJoy = true;

		size_t windowHnd = 0;
		/*
#if defined OIS_WIN32_PLATFORM
		win->getCustomAttribute("HWND", &windowHnd);
#elif defined OIS_LINUX_PLATFORM
		win->getCustomAttribute("GLXWINDOW", &windowHnd);
#endif
		*/
		win->getCustomAttribute("WINDOW", &windowHnd);
		//Both x11 and Win32 use handle of some sort..
		std::ostringstream windowHndStr;
		windowHndStr << windowHnd;

		OIS::ParamList pl;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

		inputManager_ = OIS::InputManager::createInputSystem( pl );
		keyboard_ = static_cast<OIS::Keyboard*>(inputManager_->createInputObject( OIS::OISKeyboard, bufferedKeys ));
		keyboard_->setEventCallback(this);
		mouse_ = static_cast<OIS::Mouse*>(inputManager_->createInputObject( OIS::OISMouse, bufferedMouse ));
		mouse_->setEventCallback(this);
		try
		{
			joy_ = static_cast<OIS::JoyStick*>(inputManager_->createInputObject( OIS::OISJoyStick, bufferedJoy ));
			joy_->setBuffered(false);
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
		inputManager_->destroyInputObject( joy_ );
		//delete eventProcessor_;
	}


	void O3dInputBridge
	::step() {
		if(joy_) {
			joy_->capture();

			int buttonCount = joy_->buttons();
			int buttons = 0;
			for(int i = 0; i < buttonCount; ++i) {
				int mask = (1L << i);
				if(joy_->getJoyStickState().buttonDown(i)) {
					buttons |= mask;
				}
				if((buttons & mask) != (oldJoyButtons_ & mask)) {
					if((buttons & mask) == 0) {
						O3dSchema::inputManager().active()->joyButtonReleased(i);
					}
					else {
						O3dSchema::inputManager().active()->joyButtonPressed(i);
					}
				}

			}
			oldJoyButtons_ = buttons;

			//Check to see if any of the axes values have changed.. if so send events
			for( int i = 0; i < 8; ++i )
			{
				OIS::JoyStickEvent temp(joy_, joy_->getJoyStickState());
				// TODO: Check - did move?
				O3dSchema::inputManager().active()->axisMoved( temp, i );
				// TODO: Check - did move?
				O3dSchema::inputManager().active()->sliderMoved( temp, i );
			}

		}
		if(keyboard_) {
			keyboard_->capture();
		}
		if(mouse_) {
			mouse_->capture();
		}
	}


	bool O3dInputBridge
	::keyPressed(const OIS::KeyEvent& e) {
		// Tab flips focus on console
		/*
		if(e->key == OIS::KC_TAB) {
			if(O3dSchema::console) {
				O3dSchema::console->flipFocus();
			}
		}
		*/

		switch(e.key) {
		case OIS::KC_ESCAPE:
			SimSchema::simEngine.setGameOver(true);
			break;
		case OIS::KC_SCROLL:
			O3dSchema::worldManager->flipDebugOverlay();
			break;
		case OIS::KC_PAUSE:
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

		case OIS::KC_LSHIFT:
			O3dSchema::inputManager().setModifier(InputManager::LSHIFT, true);
			break;

		case OIS::KC_RSHIFT:
			O3dSchema::inputManager().setModifier(InputManager::RSHIFT, true);
			break;

		case OIS::KC_RCONTROL:
			O3dSchema::inputManager().setModifier(InputManager::RCTRL, true);
			break;

		case OIS::KC_LCONTROL:
			O3dSchema::inputManager().setModifier(InputManager::LCTRL, true);
			break;
		}

		// If console is focused, send input there
		bool isProcessed = false;
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->keyPressed(&e);
			isProcessed = true;
		}
		// If not, send to game input handler
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->keyPressed(&e);
			isProcessed = true;
		}


		return true;
	}


	bool O3dInputBridge
	::keyReleased(const OIS::KeyEvent& e) {
		switch(e.key) {
		case OIS::KC_LSHIFT:
			O3dSchema::inputManager().setModifier(InputManager::LSHIFT, false);
			break;

		case OIS::KC_RSHIFT:
			O3dSchema::inputManager().setModifier(InputManager::RSHIFT, false);

		case OIS::KC_RCONTROL:
			O3dSchema::inputManager().setModifier(InputManager::RCTRL, false);
			break;

		case OIS::KC_LCONTROL:
			O3dSchema::inputManager().setModifier(InputManager::LCTRL, false);
			break;
		}

		if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->keyReleased(&e);
		}
		return true;
	}


	bool O3dInputBridge
	::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mousePressed(&e, id);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mousePressed(&e, id);
		}
		return true;
	}


	bool O3dInputBridge
	::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseReleased(&e, id);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseReleased(&e, id);
		}
		return true;
	}

	bool O3dInputBridge
	::mouseMoved(const OIS::MouseEvent& e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseMoved(&e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseMoved(&e);
		}
		return true;
	}


	/*
	void O3dInputBridge
	::mouseDragged(const OIS::MouseEvent* e) {
		// If console is focused, send input there
		if(O3dSchema::console && O3dSchema::console->isFocused()) {
			O3dSchema::console->mouseDragged(e);
		}
		else if(O3dSchema::inputManager().active()) {
			O3dSchema::inputManager().active()->mouseDragged(e);
		}
	}
	*/


	bool O3dInputBridge
	::buttonPressed (const OIS::JoyStickEvent &arg, int button) {
		O3dSchema::inputManager().active()->buttonPressed(arg, button);
		return true;
	}

	bool O3dInputBridge
	::buttonReleased (const OIS::JoyStickEvent &arg, int button) {
		O3dSchema::inputManager().active()->buttonReleased(arg, button);
		return true;
	}

	bool O3dInputBridge
	::axisMoved (const OIS::JoyStickEvent &arg, int axis) {
		O3dSchema::inputManager().active()->axisMoved(arg, axis);
		return true;
	}

	bool O3dInputBridge
	::sliderMoved (const OIS::JoyStickEvent &arg, int axis) {
		O3dSchema::inputManager().active()->sliderMoved(arg, axis);
		return true;
	}


	bool O3dInputBridge
	::povMoved (const OIS::JoyStickEvent &arg, int axis) {
		O3dSchema::inputManager().active()->sliderMoved(arg, axis);
		return true;
	}
}
