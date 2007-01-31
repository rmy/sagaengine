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
#include "./area/O3dManager.hpp"
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
		delete O3dSchema::console;
		O3dSchema::console = 0;
		LogMsg("Destroyed console");

		delete O3dSchema::speechBubble;
		O3dSchema::speechBubble = 0;
		LogMsg("Destroyed speech bubble handler");

		delete O3dSchema::root;
		O3dSchema::root = 0;
		LogMsg("Destroyed ogre root");
	}

	void RenderEngine
	::postSimTickEvent(long when) {
		O3dSchema::taskList.perform(1);
	}


	void RenderEngine
	::renderFrame() {
		// WorldManager::frameStarted is called before rendering
		O3dSchema::root->renderOneFrame();
		// WorldManager::frameEnded is called after rendering
		if(inputBridge_) {
			inputBridge_->step();
		}	
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
		bool carryOn = configure();
		if (!carryOn) return false;

		setupResources();

		// Create any resource listeners (for loading screens)
		createResourceListener();

		// Load resources
		loadResources();

		const Ogre::RenderSystemCapabilities* caps = O3dSchema::root->getRenderSystem()->getCapabilities();
		if (!caps->hasCapability(RSC_VERTEX_PROGRAM)) {
			LogMsg("Your card does not support vertex programs.");
		}

		// Setup animation default
		Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
		Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);
		LogMsg("Initialized interpolation of animations.");

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

		if(O3dSchema::console) {
			// Give application chance to create gui from
			// xml in a renderEventListener before creating
			// console window
			O3dSchema::console->createConsoleWindow();
			LogMsg("Created applications CEGUI.");
		}

		// Create bridge between input
		createInputBridge();
		
		// Configure this from file
		//chooseSceneManager();
		//createCamera();
		//createViewports();

		return true;
	}


	void RenderEngine
	::cleanup(void) {
		LogMsg("Shutting down resource group manager");
		ResourceGroupManager::getSingleton().shutdownAll();
		/*
		const StringVector& s = ResourceGroupManager::getSingleton().getResourceGroups();
		for(int i = 0; i < s.size(); ++i) {
			LogMsg("Destroying resource group: " << s[i].c_str());
			ResourceGroupManager::getSingleton().destroyResourceGroup(s[i]);
		}
		*/


		delete inputBridge_;
		inputBridge_ = 0;
		LogMsg("Destroyed input bridge");

		O3dSchema::sceneManager = 0;
		LogMsg("Destroyed world manager and scene manager");

		delete O3dSchema::raySceneQuery;
		O3dSchema::raySceneQuery = 0;
		LogMsg("Destroyed ray scene query");
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
			LogMsg("Loaded config");
		}
		bool gotConfig = false;
		try {
			gotConfig = O3dSchema::root->showConfigDialog();
		}
		catch(...) {
			// Probably failed to write config
			gotConfig = true;
		}
		if(gotConfig) {
			// If returned true, user clicked OK so initialise
			// Here we choose to let the system create a default rendering window by
			// passing 'true'
			LogMsg("Got requested config");
			try {
				O3dSchema::root->saveConfig();
				LogMsg("Saved config");
			}
			catch(...) {
				// Probably running from non-writeable media
				LogMsg("Couldn't write config");
			}
			O3dSchema::window = O3dSchema::root->initialise(true);
			return true;
		}
		else {
			LogMsg("Config canceled by user");
			return false;
		}
	}


	/// Method which will define the source of resources (other than current folder)
	void RenderEngine
	::setupResources(void) {
		// Load resource paths from config file
		ConfigFile cf;
		Ogre::String dataPath;
		if(IoSchema::dataPath)
			dataPath += IoSchema::dataPath;
		cf.load(dataPath + "/ogre/resources.cfg");

		// Go through all sections & settings in the file
		ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements()) {
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i) {
				typeName = i->first;
				archName = dataPath + i->second;
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
