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


#include "O3dPre.H"
#include "O3dInitHandler.hpp"
#include "../schema/O3dSchema.hpp"
#include "../system/O3dClock.hpp"
#include "../RenderEngine.hpp"
#include "../io/O3dFileManager.hpp"
#include "../area/WorldManager.hpp"

using namespace se_core;

namespace se_ogre {

	O3dInitHandler
	::O3dInitHandler() {
		SimSchema::initListeners().addListener(*this);
	}


	O3dInitHandler
	::~O3dInitHandler() {
		SimSchema::initListeners().removeListener(*this);
	}


	void O3dInitHandler
	::initEngineEvent() {
		//
		SimSchema::realClock = new O3dClock();

		// Register a file manager
		// (Could have been a network loader, or anything else.)
		IoSchema::fileManager = new se_ogre::O3dFileManager("datapath.txt");
		IoSchema::fileManager->init();

		// Create RenderEngine object
		O3dSchema::renderEngine = new RenderEngine();

		// Setup scene
		if(!O3dSchema::renderEngine->setup()) {
			// Failure
			LogFatal("RenderEngine setup failed");
			return; // false;
		}

		// Make WorldManager listen to sagaengine core events
		ClientSchema::clientListeners.addListener(*O3dSchema::worldManager);
		LogMsg("Added world manager as SagaEngine client listener");

		// Make WorldManager listen to Ogre render events
		Ogre::Root::getSingleton().addFrameListener(O3dSchema::worldManager);
		LogMsg("Added world manager as Ogre frame listener");		
	}


	void O3dInitHandler
	::cleanupEngineEvent() {
		// Make WorldManager listen to sagaengine core events
		ClientSchema::clientListeners.removeListener(*O3dSchema::worldManager);
		LogMsg("Removed SagaEngine client listener");

		// Make WorldManager listen to Ogre render events
		Ogre::Root::getSingleton().removeFrameListener(O3dSchema::worldManager);
		LogMsg("Removed Ogre frame listener");

		// Cleared world
		O3dSchema::worldManager->clearWorld();
		LogMsg("Cleared world");
		
		// Clear scene graph
		O3dSchema::sceneManager->clearScene();
		LogMsg("Cleared scene");

		delete O3dSchema::renderEngine;
		O3dSchema::renderEngine = 0;

		delete SimSchema::realClock;
		SimSchema::realClock = 0;

		IoSchema::fileManager->cleanup();
		delete IoSchema::fileManager;
		IoSchema::fileManager = 0;
	}


	void O3dInitHandler
	::initGameEvent() {
		Assert(O3dSchema::renderEngine);
		SimSchema::engineListeners().addListener(*O3dSchema::renderEngine);
	}


	void O3dInitHandler
	::cleanupGameEvent() {
		Assert(O3dSchema::renderEngine);
		SimSchema::engineListeners().removeListener(*O3dSchema::renderEngine);
	}

}
