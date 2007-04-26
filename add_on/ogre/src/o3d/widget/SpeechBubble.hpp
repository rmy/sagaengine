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


#ifndef o3d_widget_SpeechBubble_hpp
#define o3d_widget_SpeechBubble_hpp

#include "O3dPre.hpp"
#include "sim/message/MessageListener.hpp"
#include "o3d/input/InputHandler.hpp"
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>

namespace se_ogre {
	class _SeOgreExport SpeechBubble : public se_core::MessageListener, InputHandler {
	public:
		SpeechBubble();
		virtual ~SpeechBubble();

		bool init();
		void cleanup();

		void infoEvent(char* text);
		void speechEvent(se_core::Actor& speaker, const char* messageName);
		void trackUserFeedback();

		void keyPressed(Ogre::KeyEvent* e) {
			trackUserFeedback();
		}
		bool joyButtonPressed (int button) {
			trackUserFeedback();
			return true;
		}
		void mousePressed(Ogre::MouseEvent* e) {
			trackUserFeedback();
		}

		void keyReleased(Ogre::KeyEvent* e) {
			if(!below()) return;
			below()->keyReleased(e);
		}

		bool joyButtonReleased (int button) {
			if(!below()) return true;
			return below()->joyButtonReleased(button);
		}

		void mouseReleased(Ogre::MouseEvent* e) {
			if(!below()) return;
			below()->mouseReleased(e);
		}

	private:
		se_core::Actor* speaker_;
		Ogre::Overlay* overlay_;
		Ogre::OverlayElement* caption_;
	};
}

#endif
