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
#include <OgreKeyEvent.h>
#include <OgreMouseEvent.h>
#include <OgreEventListeners.h>
#include <OIS.h>

namespace se_ogre {
	// Event handler to add ability to alter curvature
	class _SeOgreExport O3dInputBridge 
		: public Ogre::KeyListener
		, public Ogre::MouseListener
		, public Ogre::MouseMotionListener
		, public OIS::JoyStickListener {

	public:
		O3dInputBridge(Ogre::RenderWindow* win);
		~O3dInputBridge();

		void keyClicked(Ogre::KeyEvent* e) {}
		void keyPressed(Ogre::KeyEvent* e);
		void keyReleased(Ogre::KeyEvent* e);

		void mousePressed(Ogre::MouseEvent* e);
		void mouseReleased(Ogre::MouseEvent* e);
		void mouseClicked(Ogre::MouseEvent* e) {}
		void mouseEntered(Ogre::MouseEvent* e) {}
		void mouseExited(Ogre::MouseEvent* e) {}

		void mouseMoved(Ogre::MouseEvent* e);
		void mouseDragged(Ogre::MouseEvent* e);
		void mouseDragMoved(Ogre::MouseEvent* e) {}

		bool buttonPressed (const OIS::JoyStickEvent &arg, int button);
		bool buttonReleased (const OIS::JoyStickEvent &arg, int button);
		bool axisMoved (const OIS::JoyStickEvent &arg, int axis);
		bool sliderMoved (const OIS::JoyStickEvent &, int) { return true; }
		bool povMoved (const OIS::JoyStickEvent &, int) { return true; }

		void step();

		void initOIS();

	private: // Attributes
		//
		OIS::Mouse* mouse_;
		OIS::Keyboard* keyboard_;
		OIS::JoyStick* joy_;

		int oldJoyButtons_;

		//
		Ogre::EventProcessor* eventProcessor_;

		//
		int keyState_;
	};
}

#endif
