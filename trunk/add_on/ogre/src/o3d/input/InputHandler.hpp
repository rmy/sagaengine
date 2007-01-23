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


#ifndef o3d_input_InputHandler_hpp
#define o3d_input_InputHandler_hpp

#include "O3dPre.hpp"


namespace se_ogre {
	class _SeOgreExport InputHandler {
	public:
		InputHandler(const char* name);
		virtual ~InputHandler();

		virtual bool moveCamera(float stepDelta) { return false; }

		virtual void keyPressed(Ogre::KeyEvent* e) {};
		virtual void keyReleased(Ogre::KeyEvent* e) {}
		virtual void mousePressed(Ogre::MouseEvent* e) {e->consume();}
		virtual void mouseReleased(Ogre::MouseEvent* e) {e->consume();}
		virtual void mouseDragged(Ogre::MouseEvent* e) {}
		virtual void mouseMoved(Ogre::MouseEvent* e) {}


		virtual bool joyButtonPressed (int button) { return false; }
		virtual bool joyButtonReleased (int button) { return false; }
		virtual bool buttonPressed (const OIS::JoyStickEvent &arg, int button) { return false; }
		virtual bool buttonReleased (const OIS::JoyStickEvent &arg, int button) { return false; }
		virtual bool axisMoved (const OIS::JoyStickEvent &arg, int axis) { return false; }
		virtual bool sliderMoved (const OIS::JoyStickEvent &, int) { return false; }
		virtual bool povMoved (const OIS::JoyStickEvent &, int) { return false; }

/*
		virtual void keyPressed(Ogre::KeyEvent* e) = 0;
		virtual void keyReleased(Ogre::KeyEvent* e) = 0;

		virtual void mousePressed(Ogre::MouseEvent* e) = 0;
		virtual void mouseReleased(Ogre::MouseEvent* e) = 0;

		virtual void mouseMoved(Ogre::MouseEvent* e) = 0;
		virtual void mouseDragged(Ogre::MouseEvent* e) = 0;
*/

		void grabFocus();
		void loseFocus();
		bool hasFocus();
		const char* name();

	private:
		const char* name_;
	};
}

#endif
