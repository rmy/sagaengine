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
#include "RenderEngine.hpp"
#include "./area/WorldManager.hpp"
#include "./config/o3d_config.hpp"
#include "./event/RenderEventListeners.hpp"
#include "./input/O3dInputBridge.hpp"
#include "./input/InputHandler.hpp"
#include "./input/InputManager.hpp"
#include "./input/Console.hpp"
#include "./schema/O3dSchema.hpp"
#include "./thing/ThingEntity.hpp"
#include "./widget/all.hpp"
#include "./io/all.hpp"

using namespace Ogre;
using namespace se_core;


namespace se_ogre {
	RenderEngine* RenderEngine
	::singleton() {
		Assert(O3dSchema::renderEngine);
		return O3dSchema::renderEngine;
	}


	RenderEngine
	::RenderEngine(se_ogre::ConsoleHandler* consoleHandler)
			: inputBridge_(0) {
		O3dSchema::root = new Root();
		LogMsg("Created Ogre root");

		// Create speech listener object
		O3dSchema::speechBubble = new SpeechBubble();
		LogMsg("Created speech bubble handler");

		if(IS_CONSOLE_ENABLED) {
			O3dSchema::console = new Console();
			if(consoleHandler) {
				O3dSchema::console->setConsoleHandler(consoleHandler);
			}
			LogMsg("Created console");
		}
	}


	RenderEngine
	::~RenderEngine() {
		if(O3dSchema::playerCamera) {
			O3dSchema::sceneManager->destroyCamera(O3dSchema::playerCamera);
			O3dSchema::playerCamera = 0;
			LogMsg("Destroyed camera");
		}
		delete inputBridge_;
		inputBridge_ = 0;
		LogMsg("Destroyed input bridge");

		delete O3dSchema::console;
		O3dSchema::console = 0;
		LogMsg("Destroyed console");

		delete O3dSchema::worldManager;
		O3dSchema::worldManager = 0;
		O3dSchema::sceneManager = 0;
		LogMsg("Destroyed world manager and scene manager");

		delete O3dSchema::raySceneQuery;
		O3dSchema::raySceneQuery = 0;
		LogMsg("Destroyed ray scene query");

		delete O3dSchema::root;
		O3dSchema::root = 0;
		LogMsg("Destroyed ogre root");
	}



	void RenderEngine
	::renderFrame() {
		// WorldManager::frameStarted is called before rendering
		O3dSchema::root->renderOneFrame();
		// WorldManager::frameEnded is called after rendering
	}


	void RenderEngine
	::screenshot(int screenShotId) {
		// Will render frame buffer to file
		char buffer[256];
		sprintf(buffer, "screenshot%05d.jpg", screenShotId);
		Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getTarget()->writeContentsToFile(buffer);
	}

	void RenderEngine
	::resetGameClock(void) {
		O3dSchema::gameClock = 0;
	}


	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	bool RenderEngine
	::setup(void) {
		chooseSceneManager();

		bool carryOn = configure();
		if (!carryOn) return false;

		createCamera();
		createViewports();
		setupResources();


		// Create any resource listeners (for loading screens)
		createResourceListener();

		// Load resources
		loadResources();

		const Ogre::RenderSystemCapabilities* caps = O3dSchema::root->getRenderSystem()->getCapabilities();
		if (!caps->hasCapability(RSC_VERTEX_PROGRAM)) {
			LogMsg("Your card does not support vertex programs.");
		}

		// Create the scene
		createScene();
		if(IS_CONSOLE_ENABLED) {
			try {
				O3dSchema::console->setupGuiSystem();
				LogMsg("Setup GUI system");
			}
			catch(...) {
				LogMsg("Failed initializeing console window. Console unavailable.");
				delete O3dSchema::console;
				O3dSchema::console = 0;
			}
		}

		O3dSchema::renderEventListeners().castInit();
		LogMsg("Cast init event to render event listeners");

		if(O3dSchema::console) {
			// Give application chance to create gui from
			// xml in a renderEventListener before creating
			// console window
			O3dSchema::console->createConsoleWindow();
			LogMsg("Created applications CEGUI.");
		}

		createInputBridge();

		renderFrame();

		return true;
	}


	void RenderEngine
	::chooseSceneManager(void) {
		O3dSchema::worldManager = new WorldManager();
		LogMsg("Created world manager");

        // Create the SceneManager, in this case a generic one
        O3dSchema::sceneManager = O3dSchema::root->createSceneManager(ST_GENERIC, "gameSM");
		LogMsg("Created scene manager: " << O3dSchema::sceneManager->getName().c_str());
		// My laptop ATI Mobility Radeon 9200 needs this initial ambient light
		// even if it is changed later (or else everything goes dark)
		O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
		Ogre::SceneNode* node = O3dSchema::sceneManager->getRootSceneNode()->createChildSceneNode("MainSceneNode");
		// LogMsg("Create 
		node->createChildSceneNode("ThingSceneNode");
		//node->createChildSceneNode("AreaSceneNode");
	}


	void RenderEngine
	::createCamera(void) {
		// Create the camera
		O3dSchema::playerCamera = O3dSchema::sceneManager->createCamera("PlayerCam");
		LogMsg("Created player camera");

		// Position it at 500 in Z direction
		O3dSchema::playerCamera->setPosition(Ogre::Vector3(128,25,128));
		// Look back along -Z
		O3dSchema::playerCamera->lookAt(Ogre::Vector3(0, 0, -300));
		O3dSchema::playerCamera->setNearClipDistance( 0.05 );
		O3dSchema::playerCamera->setFarClipDistance( 9750 );
		O3dSchema::playerCamera->setFOVy(Radian(Degree(45)));
		/*
		O3dSchema::playerCamera->setPolygonMode(PM_WIREFRAME);
		O3dSchema::playerCamera->setPolygonMode(PM_SOLID);
		O3dSchema::playerCamera->setPolygonMode(PM_POINTS);
		*/

		// TerrainSceneManager call
		//O3dSchema::sceneManager->setPrimaryCamera(O3dSchema::playerCamera);
	}


	void RenderEngine
	::createScene(void) {
		LogMsg("Creating scene");
		// Fog and background colour
		//ColourValue fadeColour(0.56, 0.56, 0.75);

        // Setup animation default
        Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
        Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);

		LogMsg("Initialized interpolation of animations.");
	}


	void RenderEngine
	::destroyScene(void) {
		O3dSchema::renderEventListeners().castCleanup();
	}


	// Create new frame listener
	void RenderEngine
	::createInputBridge(void) {
		inputBridge_= new O3dInputBridge(O3dSchema::window);
		LogMsg("Created input bridge");
	}


	/** Configures the application - returns false if the user chooses
	 * to abandon configuration. */
	bool RenderEngine
	::configure(void) {
		// Show the configuration dialog and initialise the system
		// You can skip this and use root.restoreConfig() to load configuration
		// settings if you were sure there are valid ones saved in ogre.cfg
		if(O3dSchema::root->restoreConfig()) {
			O3dSchema::window = O3dSchema::root->initialise(true, "Game Render Window");
			LogMsg("Loaded config");
			return true;
		}
		else if(O3dSchema::root->showConfigDialog()) {
			// If returned true, user clicked OK so initialise
			// Here we choose to let the system create a default rendering window by
			// passing 'true'
			O3dSchema::root->saveConfig();
			O3dSchema::window = O3dSchema::root->initialise(true);
			LogMsg("Got requested config");
			return true;
		}
		else {
			LogMsg("Config canceled by user");
			return false;
		}
	}



	void RenderEngine
	::createViewports(void) {
		// Create one viewport, entire window
		Viewport* vp = O3dSchema::window->addViewport(O3dSchema::playerCamera);
		vp->setBackgroundColour(ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		O3dSchema::playerCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

		LogMsg("Created Ogre viewport");
	}


	/// Method which will define the source of resources (other than current folder)
	void RenderEngine
	::setupResources(void) {
		// Load resource paths from config file
		ConfigFile cf;
		cf.load("resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements()) {
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i) {
				typeName = i->first;
				archName = i->second;
				ResourceGroupManager::getSingleton()
					.addResourceLocation(archName, typeName, secName);
			}
		}
		LogMsg("Initialised resources.");
	}


	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	void RenderEngine
	::loadResources(void) {
		// Initialise, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}


}
