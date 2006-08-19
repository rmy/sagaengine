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
#include "O3dSchema.hpp"
#include "o3d/system/O3dClock.hpp"
#include "o3d/RenderEngine.hpp"
#include "o3d/area/WorldManager.hpp"
#include "o3d/event/RenderEventListeners.hpp"
#include "o3d/thing/ThingMOManager.hpp"
#include "o3d/widget/SpeechBubble.hpp"
#include "o3d/io/all.hpp"
#include "o3d/input/InputManager.hpp"
#include "o3d/input/Console.hpp"


using namespace Ogre;
using namespace se_client;
using namespace se_core;

namespace se_ogre {
	namespace O3dSchema {
		ThingMOManager thingMOManager;

		RaySceneQuery* raySceneQuery = 0;
		ThingMOList thingMOList(2);

		Ogre::Root* root = 0;
		Ogre::SceneManager* sceneManager = 0;
		WorldManager* worldManager = 0;
		Ogre::Root* ogreRoot = 0;
		Ogre::Camera* playerCamera = 0;
		Ogre::RenderWindow* window = 0;

		SpeechBubble* speechBubble = 0;
		RenderEngine* renderEngine = 0;

		InputManager& inputManager() {
			static InputManager im;
			return im;
		}
		Console* console = 0;

		RenderEventListeners& renderEventListeners() {
			static RenderEventListeners rel;
			return rel;
		}

		float gameClock = 0;
		struct _SeOgreExport AutoInit : public se_core::InitListener {
			AutoInit() {
				// Auto create and register instance of this parsers
				static O3dThingParserModule o3dThingPM(se_core::IoSchema::parser());
				static O3dConfigParserModule o3dConfigPM(se_core::IoSchema::parser());
				static O3dAreaParserModule o3dAreaPM(se_core::IoSchema::parser());

				SimSchema::initListeners().addListener(*this);
				LogMsg("Registered Ogre add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogMsg("Cleaned up Ogre add-on");
			}

			void initEngineEvent() {
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
			void cleanupEngineEvent() {
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
			void initGameEvent() {
				Assert(O3dSchema::renderEngine);
				SimSchema::engineListeners().addListener(*O3dSchema::renderEngine);
			}
			void cleanupGameEvent() {
				Assert(O3dSchema::renderEngine);
				SimSchema::engineListeners().removeListener(*O3dSchema::renderEngine);
			}

		} autoInit;
	}
}

