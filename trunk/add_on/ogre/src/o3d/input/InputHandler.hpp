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
#include "OIS.h"


namespace se_ogre {
	class _SeOgreExport InputHandler {
	public:
		InputHandler(const char* name);
		virtual ~InputHandler();

		virtual bool moveCamera(float stepDelta) { return false; }

		virtual void keyPressed(const OIS::KeyEvent* e) {};
		virtual void keyReleased(const OIS::KeyEvent* e) {}
		virtual void mousePressed(const OIS::MouseEvent* e, int button) {}
		virtual void mouseReleased(const OIS::MouseEvent* e, int button) {}
		virtual void mouseDragged(const OIS::MouseEvent* e) {}
		virtual void mouseMoved(const OIS::MouseEvent* e) {}


		virtual bool joyButtonPressed (int button) { return true; }
		virtual bool joyButtonReleased (int button) { return true; }
		virtual bool buttonPressed (const OIS::JoyStickEvent &arg, int button) { return true; }
		virtual bool buttonReleased (const OIS::JoyStickEvent &arg, int button) { return true; }
		virtual bool axisMoved (const OIS::JoyStickEvent &arg, int axis) { return true; }
		virtual bool sliderMoved (const OIS::JoyStickEvent &, int) { return true; }
		virtual bool povMoved (const OIS::JoyStickEvent &, int) { return true; }

		InputHandler* below();
/*
		virtual void keyPressedconst OIS::KeyEvent* e) = 0;
		virtual void keyReleased(const OIS::KeyEvent* e) = 0;

		virtual void mousePressed(const OIS::MouseEvent* e) = 0;
		virtual void mouseReleased(const OIS::MouseEvent* e) = 0;

		virtual void mouseMoved(const OIS::MouseEvent* e) = 0;
		virtual void mouseDragged(const OIS::MouseEvent* e) = 0;
*/

		bool isShiftDown() const;
		bool isAltDown() const;
		bool isControlDown() const;
		bool isWinDown() const;

		void grabFocus();
		void loseFocus();
		bool hasFocus();

		virtual void grabbedFocusEvent() {}
		virtual void lostFocusEvent() {}
		const char* name();

	private:
		const char* name_;
	};
}

#endif
