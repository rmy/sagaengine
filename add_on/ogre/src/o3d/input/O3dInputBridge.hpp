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


#ifndef O3dInputBridge_hpp
#define O3dInputBridge_hpp

#include "O3dPre.hpp"
#include <OIS.h>

namespace se_ogre {
	// Event handler to add ability to alter curvature
	class _SeOgreExport O3dInputBridge 
		: public OIS::KeyListener
		, public OIS::MouseListener
		, public OIS::JoyStickListener {

	public:
		O3dInputBridge(Ogre::RenderWindow* win);
		~O3dInputBridge();

		//void keyClicked(const OIS::KeyEvent& e) {}
		bool keyPressed(const OIS::KeyEvent& e);
		bool keyReleased(const OIS::KeyEvent& e);

		bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id);
		//void mouseClicked(const OIS::MouseEvent* e) {}
		//void mouseEntered(const OIS::MouseEvent* e) {}
		//void mouseExited(const OIS::MouseEvent* e) {}

		bool mouseMoved(const OIS::MouseEvent& e);
		//void mouseDragged(const OIS::MouseEvent* e);
		//void mouseDragMoved(const OIS::MouseEvent* e) {}

		bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
		bool buttonReleased (const OIS::JoyStickEvent &arg, int button);
		bool axisMoved (const OIS::JoyStickEvent &arg, int axis);
		bool sliderMoved (const OIS::JoyStickEvent &, int);
		bool povMoved (const OIS::JoyStickEvent &, int);

		void step();

		void initOIS();

		bool isShiftDown() const { return isLeftShiftDown_ || isRightShiftDown_; }

	private: // Attributes
		OIS::InputManager* inputManager_;
		//
		OIS::Mouse* mouse_;
		OIS::Keyboard* keyboard_;
#ifdef SUPPORT_JOY
		OIS::JoyStick* joy_;
#endif

		int oldJoyButtons_;

		//
		int keyState_;

		bool isLeftShiftDown_, isRightShiftDown_;
	};
}

#endif
