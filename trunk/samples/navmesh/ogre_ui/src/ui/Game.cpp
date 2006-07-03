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


#include "OgreUiPre.H"
#include "Game.hpp"


using namespace se_core;
using namespace se_ogre;


namespace ui {
	Game
	::Game() {
	}


	Game
	::~Game() {
	}


	void Game
	::go() {
		while(true) {
			// Any in game events caused the game to end?
			if(SimSchema::simEngine.isGameOver()) {
				// Will cause renderloop to end
				return;
			}

			// Translate game clock to SagaEngine format
			long when = SimSchema::realClock->millis();

			// Perform next AI steps if any is waiting
			SimSchema::simEngine.step(when);

			// TODO: Should render on post tick event.
			O3dSchema::renderEngine->renderFrame();
		}
	}


	void Game
	::init() {
		// Init SagaEngine Core

		// Allow catch-up of AI to hardware clock
		SimSchema::simEngine.setMultiplePerformsPerStepEnabled(true);

		// Init simulation engine for new game
		SimSchema::simEngine.initGame();

		// Place creatures in areas
		IoSchema::fileManager->loadDirectory("game/area/thing/");

		//
		IoSchema::fileManager->loadBatch("game/init.txt");
	}


	void Game
	::cleanup() {
		// Hide game frame
		SimSchema::simEngine.cleanupGame();
	}
}
