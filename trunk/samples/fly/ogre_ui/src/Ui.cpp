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


#include "OgreUiPre.hpp"
#include "../include/game_ui.hpp"
#include "ui/input/FlyControls.hpp"
#include "io/stream/FileManager.hpp"

using namespace se_core;
using namespace se_ogre;

namespace ui {
	Ui
	::Ui() {
	}


	bool Ui
	::init() {
		// Load all files mentioned in "ogre/init_startup"
		//IoSchema::fileManager->loadBatch("ogre/init_startup.txt");
		// Setup ogre before initialising SagaEngine
		//if(!RenderEngine::init()) {
			// Failure
		//	return false;
		//}

		// Load bindings between (ogre) 3d models and (core) things
		IoSchema::fileManager->loadDirectory("ogre/thing/");

		return true;
	}


	void Ui
	::cleanup() {
	}

}
