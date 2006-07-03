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

#include "O3dPre.H"

namespace se_ogre {
	// Event handler to add ability to alter curvature
	class O3dInputBridge : public Ogre::KeyListener
						 , public Ogre::MouseListener
						 , public Ogre::MouseMotionListener {
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


	private: // Attributes
		//
		Ogre::EventProcessor* eventProcessor_;

		//
		int keyState_;
	};
}

#endif
