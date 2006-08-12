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


#ifndef Ui_hpp
#define Ui_hpp

#include "OgreUiPre.hpp"

namespace ui {

	/**
	 * This is a small class that the game logic module
	 * uses to start the user interface. This is all
	 * that the game logic module should need to include
	 * for ui, to speed up compilation when compiling
	 * game logic.
	 */
	class _GameUiExport Ui {
	public:
		Ui();
		bool init();
		void cleanup();

	};

}


#endif

