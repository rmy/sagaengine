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
#include "Ui.hpp"
#include "ui/input/GameControls.hpp"
#include "ui/material/Sunlight.hpp"

using namespace se_core;
using namespace se_ogre;

namespace ui {
	Ui
	::Ui() {
	}


	bool Ui
	::init() {
		static Sunlight sunlight;

		// Load bindings between (ogre) 3d models and (core) things
		IoSchema::fileManager->loadDirectory("ogre/thing/");

		// Load 3d models
		return true;
	}


	void Ui
	::cleanup() {
		RenderEngine::singleton()->cleanup();
	}

}
