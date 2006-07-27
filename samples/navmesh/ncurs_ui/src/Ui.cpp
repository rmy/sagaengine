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


#include "NCursUiPre.H"
#include "../include/platform_ui.hpp"
#include "ui/Game.hpp"

using namespace se_core;
using namespace se_ogre;

namespace ui {
	Ui
	::Ui() : doContinue_(true) {
	}


	bool Ui
	::init() {
		// Load all files mentioned in "ogre/init_startup"
		//IoSchema::fileManager->loadBatch("ogre/init_startup.txt");
		// Setup ogre before initialising SagaEngine
		if(!RenderEngine::init()) {
			// Failure
			return false;
		}

		// Create a game controller object
		//gameControls_ = new DirectControl3rdP();
		//gameControls_ = new BlobberControls();

		// Give focus to the game controller
		//gameControls_->grabFocus();

		// Load bindings between (ogre) 3d models and (core) things
		//IoSchema::fileManager->loadDirectory("ogre/thing/");

		// Load 3d models
		return true;
	}


	void Ui
	::cleanup() {
		RenderEngine::singleton()->cleanup();
	}


	void Ui
	::go() {
		doContinue_ = true;
		menu();
		while(doContinue_) {
			game();
			menu();
		}
	}


	bool Ui
	::menu() {
		return true;
	}

	void Ui
	::game() {
		Game game;

		game.init();
		game.go();
		game.cleanup();

		doContinue_ = false;
	}
}