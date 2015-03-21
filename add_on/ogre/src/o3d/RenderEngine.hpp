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

#include "O3dPre.hpp"
#include "o3d/event/o3d_event.hpp"
#include "o3d/input/o3d_input.hpp"
#include "sim/SimListener.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include <Ogre.h>

namespace se_ogre {
	class _SeOgreExport RenderEngine : public se_core::SimListener {
	public:
		RenderEngine(se_ogre::ConsoleHandler* consoleHandler = 0);
		virtual ~RenderEngine();

		void renderEvent(long when) { renderFrame(); }
		void preSimTickEvent(long when) {}
		void postSimTickEvent(long when);


		static RenderEngine* singleton();
		bool setup(void);
		void cleanup(void);

		void skipNext() { skip_ = 3; }
		void resetSkip() { skip_ = 0; }
		void renderFrame();
		void screenshot(int screenShotId);
		void screenshot(const char* prefix);
		void loadLevelResources(const char** sections);
		void resetLevelResources();

		/// Method which will define the source of resources
		/// (other than current folder)
		void setupResources(const char* file);
		void cleanupResources(const char* file);
	protected:
		/// SagaEngine Utility methods
		void resetGameClock(void);

		/// Ogre initialisation
		bool configure(void);

		void createInputBridge(void);



		/// Optional override method where you can create resource
		/// listeners (e.g. for loading screens)
		void createResourceListener(void) {}

		/// Optional override method where you can perform resource group loading
		/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		void loadResources(void);

	protected:
		O3dInputBridge* inputBridge_;
		static const int MAX_LEVEL_RESOURCE_SECTIONS = 8;
		int levelResourceCount_;
		const char* levelResources_[ MAX_LEVEL_RESOURCE_SECTIONS ];
		int skip_;

		char configPath_[1024];
		char logPath_[1024];

	private:
#ifdef _DEBUG
		const static int fastExit = 1;
#else
		const static int fastExit = 0;
#endif
    virtual void createCamera(void)
    {
        // Create the camera
        O3dSchema::playerCamera = O3dSchema::sceneManager->createCamera("PlayerCam");

        // Position it at 500 in Z direction
        O3dSchema::playerCamera->setPosition(Ogre::Vector3(0,0,500));
        // Look back along -Z
        O3dSchema::playerCamera->lookAt(Ogre::Vector3(0,0,-300));
        O3dSchema::playerCamera->setNearClipDistance(5);

    }
    virtual void createViewports(void)
    {
        // Create one viewport, entire window
		Ogre::Viewport* vp = O3dSchema::window->addViewport(O3dSchema::playerCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        O3dSchema::playerCamera->setAspectRatio(
            Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    }

	};

}


#endif
