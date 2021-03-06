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
#include "o3d/area/O3dAreaComponent.hpp"
#include "o3d/input/Console.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"
#include "sim/SimEngine.hpp"
#include "sim/SimListeners.hpp"
#include "util/task/TaskList.hpp"
#include "io/stream/FileManager.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/area/Area.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/pos/Pos.hpp"
#include <OgreRoot.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>

using namespace Ogre;
using namespace se_client;
using namespace se_core;

namespace se_ogre {
	namespace O3dSchema {
		ThingMOManager thingMOManager;
		O3dThingComponentFactory otcf;

		RaySceneQuery* raySceneQuery = 0;
		se_core::TaskList taskList;

		TextureSetting textureSetting;
		ShadowSetting shadowSetting;

		Ogre::Root* root = 0;
		Ogre::LogManager* logManager = 0;
		Ogre::SceneManager* sceneManager = 0;
		O3dManager* worldManager = 0;
		Ogre::Camera* playerCamera = 0;
		Ogre::RenderWindow* window = 0;
		Ogre::OverlaySystem *overlaySystem = 0;

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
				static O3dThingParser o3dThingPM(se_core::IoSchema::parser());
				static O3dConfigParser o3dConfigPM(se_core::IoSchema::parser());
				static O3dAreaParser o3dAreaPM(se_core::IoSchema::parser());
				static O3dAreaComponentFactory o3dAreaCF;

				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Ogre add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Ogre add-on");
			}

			bool initEngineEvent() {
				WasHere();
				//
				SimSchema::realClock = new O3dClock();

				// Create RenderEngine object
				O3dSchema::renderEngine = new RenderEngine();
				O3dSchema::overlaySystem = new OverlaySystem();

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
				O3dSchema::console->updateConsole();
				LogDetail("Created world manager");


				// Make WorldManager listen to sagaengine core events
				//ClientSchema::clientListeners.addListener(*O3dSchema::worldManager);
				//LogDetail("Added world manager as SagaEngine client listener");

				LogDetail("Cast init event to render event listeners");
				O3dSchema::renderEventListeners().castInitEngine();


				// Init viewport
				const char* global = "logic/config/global.ogre.txt";
				if(!IoSchema::fileManager->exists(global)) {
					IoSchema::fileManager->addFileIfExists(global);
				}
				if(IoSchema::fileManager->exists(global)) {
	 				IoSchema::fileManager->load(global);
				}



				return true;
			}

			void cleanupEngineEvent() {
				WasHere();

				LogDetail("Cast init cleanup to render event listeners");
				O3dSchema::renderEventListeners().castCleanupEngine();

				if(O3dSchema::playerCamera) {
					O3dSchema::sceneManager->destroyCamera(O3dSchema::playerCamera);
					O3dSchema::playerCamera = 0;
					O3dSchema::window->removeAllViewports();
					LogDetail("Destroyed camera");
				}

				// Cleanup render engine
				O3dSchema::renderEngine->cleanup();

				delete O3dSchema::worldManager;
				O3dSchema::worldManager = 0;

				delete O3dSchema::overlaySystem;
				O3dSchema::overlaySystem = 0;

				delete O3dSchema::renderEngine;
				O3dSchema::renderEngine = 0;

				delete SimSchema::realClock;
				SimSchema::realClock = 0;
			}

			bool initGameEvent() {
				WasHere();
				Ogre::Overlay* overlay = 0;
				try {
					overlay = Ogre::OverlayManager::getSingleton().getByName("Core/Loading");
					if(overlay) {
						char buffer[512];
						if(SpeechBubble::translate("Info.LOADING", buffer)) {
							Ogre::OverlayElement* txt = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LoadingText");
							txt->setCaption(buffer);
						}
						// Make the loading overlay active
						overlay->show();
						O3dSchema::window->removeAllViewports();
						// This extra render without viewports solves some problems when a game save refuses to load
						// several times. Not sure why. Maybe gives OGRE some chance to clean up internal states after
						// resetting a scene.
						Ogre::Root::getSingleton().renderOneFrame();
						Ogre::Viewport* vp = O3dSchema::window->addViewport(O3dSchema::playerCamera);
						// Will render to loading screen
						Ogre::Root::getSingleton().renderOneFrame();
						// Make the loading overlay inactive. Next time render is called the game scene is ready to render.
						overlay->hide();
					}
					else {
						LogWarning("No loading overlay");
					}


					char resFile[128];
					sprintf(resFile, "ogre/resources%s.cfg", textureSetting.ext());
					renderEngine->setupResources(resFile);
				}
				catch(...) {
				}

				Assert(O3dSchema::renderEngine);
				O3dSchema::renderEngine->skipNext();

				// Make WorldManager listen to Ogre render events
				LogDetail("Add world manager as Ogre frame listener");
				Ogre::Root::getSingleton().addFrameListener(O3dSchema::worldManager);

				LogDetail("Register ogre add-on as sim engine listener");
				SimSchema::engineListeners().addListener(*O3dSchema::renderEngine);

				LogDetail("Cast init event to render event listeners");
				O3dSchema::renderEventListeners().castInitGame();

				return true;
			}

			void cleanupGameEvent() {
				WasHere();

				// Clear scene graph
				O3dSchema::sceneManager->clearScene();
				LogDetail("Cleared scene");

				// Cleared world
				O3dSchema::worldManager->clear();
				LogDetail("Cleared world");
		
				O3dSchema::thingMOManager.reset();
				
				// Reset scene manager
				O3dSchema::renderEngine->resetSkip();
				O3dSchema::renderEngine->resetLevelResources();

				char buffer[128];
				sprintf(buffer, "ogre/resources%s.cfg", textureSetting.ext());
				renderEngine->cleanupResources(buffer);

				// Make WorldManager listen to Ogre render events
				LogDetail("Remove Ogre frame listener");
				Ogre::Root::getSingleton().removeFrameListener(O3dSchema::worldManager);

				LogDetail("Cast init cleanup to render event listeners");
				O3dSchema::renderEventListeners().castCleanupGame();

				Assert(O3dSchema::renderEngine);
				LogDetail("Remove ogre add-on as sim engine listener");
				SimSchema::engineListeners().removeListener(*O3dSchema::renderEngine);

			}

			bool initLevelEvent() {
				WasHere();
				// Load ogre configuration
				O3dSchema::renderEngine->skipNext();

				char buffer[256];
				sprintf(buffer, "logic/config/%s.ogre.txt", SimSchema::simEngine.nextLevel());
				if(!IoSchema::fileManager->exists(buffer)) {
					IoSchema::fileManager->addFileIfExists(buffer);
				}
				if(IoSchema::fileManager->exists(buffer)) {
	 				IoSchema::fileManager->load(buffer);
				}

				ZoneAreaComponent::Ptr cZone(*ClientSchema::camera->nextPos().area());
				int c = SimSchema::areaManager.areaCount();
				for(int i = 0; i < c; ++i) {
					Area* a = SimSchema::areaManager.area(i);
					ZoneAreaComponent::Ptr aZone(*a);

					O3dAreaComponent::Ptr aO3d(*a);
					if(aZone->page().w_ == cZone->page().w_) {
						aO3d->initStaticGeometry();
					}
				}

				speechBubble->init();

				O3dSchema::taskList.performAll();
				O3dSchema::renderEventListeners().castInitLevel();

				return true;
			}


			void cleanupLevelEvent() {
				WasHere();
				O3dSchema::renderEventListeners().castCleanupLevel();

				Ogre::Overlay* overlay = 0;
				try {
					overlay = Ogre::OverlayManager::getSingleton().getByName("Core/Loading");
					if(overlay) {
						overlay->show();
						Ogre::Root::getSingleton().renderOneFrame();
						overlay->hide();
					}
					else {
						LogWarning("No loading overlay");
					}
				}
				catch(...) {
				}

				speechBubble->cleanup();

				ZoneAreaComponent::Ptr cZone(*ClientSchema::camera->nextPos().area());
				int c = SimSchema::areaManager.areaCount();
				for(int i = 0; i < c; ++i) {
					Area* a = SimSchema::areaManager.area(i);
					ZoneAreaComponent::Ptr aZone(*a);

					O3dAreaComponent::Ptr aO3d(*a);
					aO3d->cleanupStaticGeometry();
					aO3d->cleanup();
				}

				// Clear scene graph
				O3dSchema::sceneManager->clearScene();
				LogDetail("Cleared scene");
			}

		} autoInit;


		void reinit() {
			WasHere();
			autoInit.cleanupEngineEvent();
			autoInit.initEngineEvent();
		}


		void touch() {
			se_core::SimSchema::touch();
			se_core::IoSchema::touch();
			autoInit;
		}
	}
}

