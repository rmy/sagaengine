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


#include "GamePre.H"
#include "Application.hpp"
#include "../schema/GameSchema.hpp"

using namespace se_core;
using namespace ui;

namespace game {
	Application
	::Application() {
		if(!initEngine()) {
			LogFatal("Engine init failed");
		}
	}


	Application
	::~Application() {
		cleanupEngine();
	}


	bool Application
	::initEngine() {
		if(!initSagaEngine()) {
			return false;
		}

		//
		if(!initGameModule_PlatformUI()) {
			return false;
		}


		if(!GameSchema::init()) {
			return false;
		}

		Assert(IoSchema::fileManager);

		// Load language files
		IoSchema::fileManager->loadDirectory("game/lang");

		// Load scripts
		IoSchema::fileManager->loadDirectory("game/script");

		// Load thing definitions
		IoSchema::fileManager->loadDirectory("game/thing");

		// Load movement data
		IoSchema::fileManager->loadDirectory("game/area/movement/");

		// Load cutscenes
		IoSchema::fileManager->loadDirectory("game/cutscene/");

		// Load bindings between (ogre) 3d models and (core) things
		IoSchema::fileManager->loadDirectory("ogre/thing/");

		return true;
	}


	bool Application
	::initGame() {
		// Allow catch-up of AI to hardware clock
		SimSchema::simEngine.setMultiplePerformsPerStepEnabled(true);

		// Init simulation engine for new game
		SimSchema::simEngine.initGame();

		// Place creatures in areas
		IoSchema::fileManager->loadDirectory("game/area/thing/");

		//
		IoSchema::fileManager->loadBatch("game/init.txt");

		return true;
	}


	void Application
	::go() {
		if(!initGame()) {
			LogFatal("Initializing game failed");
		}

		// Enter game loop
		SimSchema::simEngine.go();

		cleanupGame();
	}


	void Application
	::cleanupGame() {
		// Hide game frame
		SimSchema::simEngine.cleanupGame();
	}


	void Application
	::cleanupEngine() {
		GameSchema::cleanup();

		cleanupSagaEngine();
	}

}
