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


#include "O3dPre.hpp"
#include "O3dSchema.hpp"
#include "o3d/system/O3dClock.hpp"
#include "o3d/RenderEngine.hpp"
#include "o3d/area/O3dManager.hpp"
#include "o3d/event/RenderEventListeners.hpp"
#include "o3d/thing/ThingMOManager.hpp"
#include "o3d/widget/SpeechBubble.hpp"
#include "o3d/io/all.hpp"
#include "o3d/input/InputManager.hpp"
#include "o3d/input/Console.hpp"
#include "o3d/area/O3dAreaComponentFactory.hpp"
#include "io/schema/IoSchema.hpp"


using namespace Ogre;
using namespace se_client;
using namespace se_core;

namespace se_ogre {
	namespace O3dSchema {
		ThingMOManager thingMOManager;

		RaySceneQuery* raySceneQuery = 0;
		ThingMOList thingMOList(2);
		se_core::TaskList taskList;

		Ogre::Root* root = 0;
		Ogre::SceneManager* sceneManager = 0;
		O3dManager* worldManager = 0;
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
				static O3dAreaComponentFactory o3dAreaCF;

				SimSchema::initListeners().addListener(*this);
				LogMsg("Registered Ogre add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogMsg("Cleaned up Ogre add-on");
			}

			bool initEngineEvent() {
				//
				SimSchema::realClock = new O3dClock();

				// Create RenderEngine object
				O3dSchema::renderEngine = new RenderEngine();

				// Setup scene
				if(!O3dSchema::renderEngine->setup()) {
					LogWarning("RenderEngine setup failed");
					// Failure
					delete O3dSchema::renderEngine;
					delete SimSchema::realClock;
					return false;
				}

				// Create world manager
				O3dSchema::worldManager = new O3dManager();
				LogMsg("Created world manager");


				// Make WorldManager listen to sagaengine core events
				//ClientSchema::clientListeners.addListener(*O3dSchema::worldManager);
				LogMsg("Added world manager as SagaEngine client listener");

				LogMsg("Cast init event to render event listeners");
				O3dSchema::renderEventListeners().castInitEngine();

				return true;
			}

			void cleanupEngineEvent() {
				LogMsg("Cast init cleanup to render event listeners");
				O3dSchema::renderEventListeners().castCleanupEngine();

				// Make WorldManager listen to sagaengine core events
				//ClientSchema::clientListeners.removeListener(*O3dSchema::worldManager);
				LogMsg("Removed SagaEngine client listener");

				// Cleanup render engine
				O3dSchema::renderEngine->cleanup();

				delete O3dSchema::worldManager;
				O3dSchema::worldManager = 0;

				delete O3dSchema::renderEngine;
				O3dSchema::renderEngine = 0;

				delete SimSchema::realClock;
				SimSchema::realClock = 0;
			}

			bool initGameEvent() {
				Assert(O3dSchema::renderEngine);

				// Make WorldManager listen to Ogre render events
				LogMsg("Add world manager as Ogre frame listener");
				Ogre::Root::getSingleton().addFrameListener(O3dSchema::worldManager);

				LogMsg("Register ogre add-on as sim engine listener");
				SimSchema::engineListeners().addListener(*O3dSchema::renderEngine);

				LogMsg("Cast init event to render event listeners");
				O3dSchema::renderEventListeners().castInitGame();

				//O3dSchema::worldManager->compileAllStaticGeometry();
				O3dSchema::taskList.perform(1024);

				return true;
			}

			void cleanupGameEvent() {
				// Make WorldManager listen to Ogre render events
				LogMsg("Remove Ogre frame listener");
				Ogre::Root::getSingleton().removeFrameListener(O3dSchema::worldManager);

				LogMsg("Cast init cleanup to render event listeners");
				O3dSchema::renderEventListeners().castCleanupGame();

				Assert(O3dSchema::renderEngine);
				LogMsg("Remove ogre add-on as sim engine listener");
				SimSchema::engineListeners().removeListener(*O3dSchema::renderEngine);

				// Cleared world
				O3dSchema::worldManager->clear();
				LogMsg("Cleared world");
		
				// Clear scene graph
				O3dSchema::sceneManager->clearScene();
				LogMsg("Cleared scene");

				if(O3dSchema::playerCamera) {
					O3dSchema::sceneManager->destroyCamera(O3dSchema::playerCamera);
					O3dSchema::playerCamera = 0;
					LogMsg("Destroyed camera");
				}

				O3dSchema::root->destroySceneManager(O3dSchema::sceneManager);
				O3dSchema::sceneManager = 0;
			}

			bool initLevelEvent() {
				return true;
			}

			void cleanupLevelEvent() {
			}

		} autoInit;

		void touch() {
			se_core::SimSchema::touch();
			se_core::IoSchema::touch();
			autoInit;
		}
	}
}

