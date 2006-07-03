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


#include "GameFrame.hpp"
#include "Renderer.hpp"
#include "util/error/Log.hpp"
#include "../canvas/Canvas.hpp"
#include "../../schema/NCursSchema.hpp"
#include "../../area/WorldManager.hpp"
#include "../../input/all.hpp"
#include "../../thing/ThingEntity.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include <cstdio>
#include <ctime>


using namespace se_core;

namespace se_ncurs {
	GameFrame
	::GameFrame()
		: isSetupDone_(false) {
		NCursSchema::renderer = new Renderer();
	}


	GameFrame
	::~GameFrame() {
		delete NCursSchema::renderer;
		NCursSchema::renderer = 0;
	}


	void GameFrame
	::go() {
		Assert(isSetupDone_);

		loop();
	}


	bool GameFrame
	::setup() {
		if(!createWorldManager()) {
			return false;
		}
		isSetupDone_ = true;
		return true;
	}


	void GameFrame
	::cleanup() {
		delete NCursSchema::worldManager;
	}


	bool GameFrame
	::createWorldManager() {
		NCursSchema::worldManager = new WorldManager();
		return true;
	}


	void GameFrame
	::loop() {
		static const int CLOCK_DIV = CLOCKS_PER_SEC / 1024;
		long when = 0;
		int k;
		while(true) {
			while((k = NCursSchema::canvas().releasedKey()) != 0) {
				NCursSchema::gameControls->keyReleased(k);
			}
			while((k = NCursSchema::canvas().pressedKey()) != 0) {
				NCursSchema::gameControls->keyPressed(k);
			}
			while(clock() / CLOCK_DIV == when) {
			}
			when = clock() / CLOCK_DIV;
			SimSchema::simEngine.step(when);
	   		if(SimSchema::simEngine.isGameOver()) {
				break;
			}
		}
	}


}

