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


#ifndef o3d_RenderEngine_hpp
#define o3d_RenderEngine_hpp

#include "O3dPre.H"
#include "o3d/event/o3d_event.hpp"
#include "o3d/input/o3d_input.hpp"


namespace se_ogre {
	extern Ogre::RaySceneQuery* raySceneQuery;

	class RenderEngine {
	public:
		RenderEngine(se_ogre::ConsoleHandler* consoleHandler = 0);
		virtual ~RenderEngine();

		static RenderEngine* singleton();
		static bool init();
		static void cleanup();
		bool initGame();
		void cleanupGame();

		void renderFrame();
		void screenshot(int screenShotId);

	protected:
		bool setup(void);
		void createScene(void);
		void destroyScene(void);

		/// SagaEngine Utility methods
		void resetGameClock(void);

		/// Ogre initialisation
		bool configure(void);
		void chooseSceneManager(void);
		void createCamera(void);
		void createInputBridge(void);

		void createViewports(void);

		/// Method which will define the source of resources
		/// (other than current folder)
		void setupResources(void);

		/// Optional override method where you can create resource
		/// listeners (e.g. for loading screens)
		void createResourceListener(void) {}

		/// Optional override method where you can perform resource group loading
		/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		void loadResources(void);

	protected:
		O3dInputBridge* inputBridge_;
	private:
#ifdef _DEBUG
		const static int fastExit = 1;
#else
		const static int fastExit = 0;
#endif

	};

}


#endif
