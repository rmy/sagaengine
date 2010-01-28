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
#include "util/task/TaskList.hpp"
#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreAnimation.h>
#include <OgreConfigFile.h>

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
			: inputBridge_(0), levelResourceCount_(0), skip_(0) {
		if(!O3dSchema::logManager) {
			O3dSchema::logManager = new LogManager();
#ifdef SE_INTERNAL
			LogManager::getSingleton().createLog("Ogre.log", true, true, false);
#else
			LogManager::getSingleton().createLog("Ogre.log", true, false, true);
			Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
#endif
		}
#ifndef _DEBUG
		O3dSchema::root = new Root("plugins.cfg", "ogre.cfg");
#else
		O3dSchema::root = new Root("plugins_d.cfg", "ogre.cfg");
#endif
		LogDetail("Created Ogre root");

		// Create speech listener object
		O3dSchema::speechBubble = new SpeechBubble();
		LogDetail("Created speech bubble handler");

		if(IS_CONSOLE_ENABLED) {
			O3dSchema::console = new Console();
			if(consoleHandler) {
				O3dSchema::console->setConsoleHandler(consoleHandler);
			}
			LogDetail("Created console");
		}
	}


	RenderEngine
	::~RenderEngine() {
		delete O3dSchema::console;
		O3dSchema::console = 0;
		LogDetail("Destroyed console");

		delete O3dSchema::speechBubble;
		O3dSchema::speechBubble = 0;
		LogDetail("Destroyed speech bubble handler");

		delete O3dSchema::root;
		O3dSchema::root = 0;
		LogDetail("Destroyed ogre root");
	}


	void RenderEngine
	::postSimTickEvent(long when) {
		if(skip_ > 0) {
			--skip_;
		}
		O3dSchema::taskList.perform(1);
	}


	void RenderEngine
	::renderFrame() {
		if(skip_ > 0) {
			return;
		}
		// WorldManager::frameStarted is called before rendering
		if(O3dSchema::window && O3dSchema::window->isActive())
			O3dSchema::root->renderOneFrame();
		// WorldManager::frameEnded is called after rendering
		if(inputBridge_) {
			inputBridge_->step();
		}
		WindowEventUtilities::messagePump();
	}


	void RenderEngine
	::screenshot(int screenShotId) {
		// Will render frame buffer to file
		char buffer[256];
		sprintf(buffer, "screenshot%05d.jpg", screenShotId);
		Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getTarget()->writeContentsToFile(buffer);
	}


	void RenderEngine
	::screenshot(const char* prefix) {
		Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getTarget()->writeContentsToTimestampedFile(prefix, "jpg");
	}


	void RenderEngine
	::resetGameClock(void) {
		O3dSchema::gameClock = 0;
	}


	/** Sets up the application - returns false if the user chooses to abandon configuration. */
	bool RenderEngine
	::setup(void) {
		setupResources("ogre/resources.cfg");

		bool carryOn = configure();
		if (!carryOn) return false;

		// Configure this from file
		//chooseSceneManager();
		//createCamera();
		//createViewports();

		TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();

		// Load resources
		loadResources();

		/*
		const Ogre::RenderSystemCapabilities* caps = O3dSchema::root->getRenderSystem()->getCapabilities();
		if (!caps->hasCapability(RSC_VERTEX_PROGRAM)) {
			LogDetail("Your card does not support vertex programs.");
		}

		// Setup animation default
		Animation::setDefaultInterpolationMode(Animation::IM_LINEAR);
		Animation::setDefaultRotationInterpolationMode(Animation::RIM_LINEAR);
		LogDetail("Initialized interpolation of animations.");

		if(IS_CONSOLE_ENABLED) {
			try {
				O3dSchema::console->setupGuiSystem();
				LogDetail("Setup GUI system");
			}
			catch(...) {
				LogDetail("Failed initializeing console window. Console unavailable.");
				delete O3dSchema::console;
				O3dSchema::console = 0;
			}
		}

		if(O3dSchema::console) {
			// Give application chance to create gui from
			// xml in a renderEventListener before creating
			// console window
			O3dSchema::console->createConsoleWindow();
			LogDetail("Created applications CEGUI.");
		}
		*/

		// Create bridge between input
		createInputBridge();

		return true;
	}


	void RenderEngine
	::cleanup(void) {
		LogDetail("Shutting down resource group manager");
		ResourceGroupManager::getSingleton().shutdownAll();

		delete inputBridge_;
		inputBridge_ = 0;
		LogDetail("Destroyed input bridge");

		O3dSchema::sceneManager = 0;
		LogDetail("Destroyed world manager and scene manager");
	}



	// Create new frame listener
	void RenderEngine
	::createInputBridge(void) {
		inputBridge_= new O3dInputBridge(O3dSchema::window);
		LogDetail("Created input bridge");
	}


	/** Configures the application - returns false if the user chooses
	 * to abandon configuration. */
	bool RenderEngine
	::configure(void) {
		// Show the configuration dialog and initialise the system
		// You can skip this and use root.restoreConfig() to load configuration
		// settings if you were sure there are valid ones saved in ogre.cfg
		if(O3dSchema::root->restoreConfig()) {
			LogDetail("Loaded config");
			try {
				O3dSchema::window = O3dSchema::root->initialise(true);
			}
			catch(...) {
			}
			if(O3dSchema::window != 0)
				return true;
		}

		try {
			std::ifstream ifs("ogre.default.cfg", std::ios::binary);
			std::ofstream ofs("ogre.cfg", std::ios::binary);

			ofs << ifs.rdbuf();
		}
		catch(...) {
		}

		if(O3dSchema::root->restoreConfig()) {
			LogDetail("Loaded config");
			try {
				O3dSchema::window = O3dSchema::root->initialise(true);
			}
			catch(...) {
			}
			if(O3dSchema::window != 0)
				return true;
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
			LogDetail("Got requested config");
			try {
				O3dSchema::root->saveConfig();
				LogDetail("Saved config");
			}
			catch(...) {
				// Probably running from non-writeable media
				LogDetail("Couldn't write config");
			}
			O3dSchema::window = O3dSchema::root->initialise(true);
			return true;
		}
		else {
			LogDetail("Config canceled by user");
			return false;
		}

		return true;
	}


	/// Method which will define the source of resources (other than current folder)
	void RenderEngine
	::setupResources(const char* file) {
		LogWarning("Loading resource file: " << file);
		// Load resource paths from config file
		ConfigFile cf;
		Ogre::String dataPath;
		if(IoSchema::dataPath)
			dataPath.append(IoSchema::dataPath);
		cf.load(dataPath + file);

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
		LogDetail("Initialised resources.");
	}

	/// Method which will define the source of resources (other than current folder)
	void RenderEngine
	::cleanupResources(const char* file) {
		// Load resource paths from config file
		ConfigFile cf;
		Ogre::String dataPath;
		if(IoSchema::dataPath)
			dataPath.append(IoSchema::dataPath);
		cf.load(dataPath + file);

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
					.removeResourceLocation(archName, secName);
			}
		}
		LogDetail("Initialised resources.");
	}


	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	void RenderEngine
	::loadResources(void) {
		const char* sections[] = { "bootstrap", "common", 0 };
		int i = 0;
		while(sections[i] != 0) {
			try {
				ResourceGroupManager::getSingleton().initialiseResourceGroup(sections[i]);
				//ResourceGroupManager::getSingleton().loadResourceGroup(sections[i]);
			} catch(...) {
			}
			++i;
		}
	}


	void RenderEngine
	::loadLevelResources(const char** sections) {
		bool isSame = true;
		for(int i = 0; i < levelResourceCount_ && sections[i] != 0; ++i) {
			if(strcmp(sections[i], levelResources_[i]) != 0) {
				isSame = false;
				break;
			}
		}
		if(isSame && sections[ levelResourceCount_ ] != 0) {
			isSame = false;
		}
		if(isSame)
			return;

		resetLevelResources();

		const char** sec = sections;
		while(*sec != 0) {
			Ogre::String s(*sec);
			try {
				ResourceGroupManager::getSingleton().initialiseResourceGroup(s + O3dSchema::textureSetting.ext());
			}
			catch(...) {
			}
			try {
				ResourceGroupManager::getSingleton().initialiseResourceGroup(s + ".all");
			}
			catch(...) {
			}
			LogWarning("Loading textures: " << *sec << O3dSchema::textureSetting.ext());
			Assert(levelResourceCount_ < MAX_LEVEL_RESOURCE_SECTIONS);
			levelResources_[ levelResourceCount_++ ] = *sec;
			++sec;
		}
	}


	void RenderEngine
	::resetLevelResources() {
		LogWarning(levelResourceCount_);
		while(levelResourceCount_ > 0) {
			const char* sec = levelResources_ [ --levelResourceCount_ ];
			Ogre::String s(sec);
			try {
				ResourceGroupManager::getSingleton().unloadResourceGroup(s + ".all");
				ResourceGroupManager::getSingleton().clearResourceGroup(s + ".all");
			}
			catch(...) {
			}
			try {
				ResourceGroupManager::getSingleton().unloadResourceGroup(s + O3dSchema::textureSetting.ext());
				ResourceGroupManager::getSingleton().clearResourceGroup(s + O3dSchema::textureSetting.ext());
			}
			catch(...) {
			}
		}
	}

}
