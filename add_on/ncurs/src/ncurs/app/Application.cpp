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


#include "Application.hpp"
#include "pc/io/all.hpp"
#include "ncurs/input/InputHandler.hpp"
#include "ncurs/schema/NCursSchema.hpp"
#include "ncurs/gfx/frame/GameFrame.hpp"
#include "util/error/Log.hpp"
#include "client/io/all.hpp"
#include "client/schema/ClientSchema.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/thing/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "io/stream/FileManager.hpp"
#include "io/stream/InputStream.hpp"
#include <cstring>


using namespace se_core;

namespace se_ncurs {

	Application
	::Application(InputHandler* gameControls)
			: gameFrame_(new GameFrame()) {
		NCursSchema::gameControls = gameControls;
		initOnce();
	}


	Application
	::~Application() {
		delete gameFrame_;
		delete NCursSchema::gameControls;
		NCursSchema::gameControls = 0;
		delete IoSchema::fileManager;
		IoSchema::fileManager = 0;
	}


	void Application
	::go(void) {
		if (!gameFrame_->setup())
			return;
		for(int g = 0; g < 3; ++g) {
			initBeforeEachGame();
			gameFrame_->go();
			cleanupAfterEachGame();
		}
		gameFrame_->cleanup();
	}


	void Application
	::loadDirectory(const char* directory) {
		int len = static_cast<int>(strlen(directory));
		for(int i = 0; i < IoSchema::fileManager->fileCount(); ++i) {
			const char* filename = IoSchema::fileManager->filename(i);
			if(strncmp(filename, directory, len) == 0) {
				LogMsg(filename);
				InputStream* is = 0;
				if(is = IoSchema::fileManager->open(filename)) {
					LogMsg(filename);
					IoSchema::parser().parse(*is, filename);
				}
				IoSchema::fileManager->close(is);
			}
		}
	}


	void Application
	::load(const char* initFilename) {
		InputStream* initIs = IoSchema::fileManager->open(initFilename);
		char filename[256];

		while(true) {
			initIs->readLine(filename, 256);
			if(filename[0] == 0 || filename[0] == 'Q') {
				break;
			}
			if(filename[0] == 0 || filename[0] == '\n' || filename[0] == ' ') {
				continue;
			}

			InputStream* is = 0;
			if(is = IoSchema::fileManager->open(filename)) {
				IoSchema::parser().parse(*is, filename);
				IoSchema::fileManager->close(is);
				continue;
			}

			LogFatal(filename);
		}
		IoSchema::fileManager->close(initIs);
	}


	void Application
	::initOnce(void) {
		IoSchema::fileManager = new PCFileManager();
		new LanguageParserModule(se_core::IoSchema::parser());
		new PCThingParserModule(se_core::IoSchema::parser());

		loadDirectory("client/lang/");
		loadDirectory("core/thing/");
		loadDirectory("ogre/thing/");
		load("ogre/init_startup.txt");
		loadDirectory("core/area/movement/");
		loadDirectory("core/cutscene/");
	}


	void Application
	::initBeforeEachGame() {
		// Do not allow catch-up of AI to the hardware clock on GBA
		// because it may hang when it is not be able to catch up at all.
		SimSchema::simEngine.setMultiplePerformsPerStepEnabled(false);

		// Set hardware clock to 0
		resetGameClock();

		// Load stuff
		loadDirectory("core/area/thing/");
		load("ncurs/init.txt");

		// Reset in game engine variable that remembers the
		// 'when' of the previous step
		SimSchema::simEngine.resetTimer();

		// Begin game with game over flag not set
		SimSchema::simEngine.setGameOver(false);
		SimSchema::didTrack = true;
		SimSchema::scriptTracker = 0;
	}


	void Application
	::cleanupAfterEachGame(void) {
		// Perform other game reset acions
		ClientSchema::player->leaveCurrentArea();
		ClientSchema::player->scheduleForDestruction();
		ClientSchema::floatingCamera->leaveCurrentArea();
		ClientSchema::floatingCamera->scheduleForDestruction();

		// Hide game frame
		//gameFrame.setVisible(false);

		SimSchema::simEngine.cleanupGame();

		delete ClientSchema::player;
		ClientSchema::player = 0;
		ClientSchema::camera = 0;
		ClientSchema::floatingCamera = 0;

		SimSchema::thingManager().performDestructions();
	}


	void Application
	::resetGameClock(void) {
		NCursSchema::gameClock = 0;
	}


	long Application
	::gameClock(void) {
		return static_cast<long>(NCursSchema::gameClock);
	}

}
