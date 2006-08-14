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


#ifndef ui_SimpleControls_hpp
#define ui_SimpleControls_hpp

#include "OgreUiPre.hpp"


namespace ui {
	// Event handler to add ability to alter curvature
	class _SimpleUiExport SimpleControls : public se_ogre::InputHandler {
	public:
		SimpleControls();
		virtual ~SimpleControls();

		void keyPressed(Ogre::KeyEvent* e);
		void keyReleased(Ogre::KeyEvent* e);

	private:
		float dirUp_, dirDown_, dirLeft_, dirRight_, dirRollLeft_, dirRollRight_;
		float dirForward_, dirBackward_;

		inline float dirForward() { return dirForward_ - dirBackward_; }
		inline float dirLR() { return dirRight_ - dirLeft_; }
		inline float dirRoll() { return dirRollRight_ - dirRollLeft_; }
		inline float dirUD() { return dirDown_ - dirUp_; }
	};
}

#endif
