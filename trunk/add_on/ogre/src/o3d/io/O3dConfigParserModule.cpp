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


#include "O3dConfigParserModule.hpp"
#include "../schema/O3dSchema.hpp"
#include "../RenderEngine.hpp"
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "O3dPre.hpp"
#include <cstring>
#include <cstdio>


using namespace se_core;


namespace se_ogre {
	O3dConfigParserModule
	::O3dConfigParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::OGRE, ParserModule::CONFIG, 1) {
	}


	void O3dConfigParserModule
	::parse(InputStream& in) {
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'S': 
				{ // SceneManager
					String sceneManager;
					in.readString(sceneManager);
					chooseSceneManager(sceneManager.get());
				}
				break;

			case 'C':
				{
					float near = in.readFloat();
					float far = in.readFloat();
					float fovy = in.readFloat();
					createCamera(near, far, fovy);
					createViewports();
					break;
				}

			case 'D': 
				try { // Dome
					String material;
					in.readString(material);
					float curvature = in.readFloat();
					float tiling = in.readFloat();
					
					Assert(O3dSchema::sceneManager 
						   && "SceneManager must be created before loading ogre config file");
					O3dSchema::sceneManager->setSkyDome(true, material.get(), curvature, tiling);
					LogMsg("Created skydome with material " << material);
				}
				catch(...) {
					LogMsg("Couldn't create skydome for ogre config file " << in.name());
				}
				break;

			case 'B': 
				try { // Box
					String material;
					in.readString(material);

					Assert(O3dSchema::sceneManager 
						   && "SceneManager must be created before loading ogre config file");
					O3dSchema::sceneManager->setSkyBox(true, material.get());
				}
				catch(...) {
					LogMsg("Couldn't create skybox for ogre config file " << in.name());
				}
				break;

			default:
				LogFatal("Unsupported code!");
			}
		}
	}


	void O3dConfigParserModule
	::chooseSceneManager(const char* sceneManager) {
        // Create the SceneManager, in this case a generic one
        O3dSchema::sceneManager = O3dSchema::root->createSceneManager(sceneManager);
		LogMsg("Created scene manager: " << O3dSchema::sceneManager->getTypeName().c_str());

		// My laptop ATI Mobility Radeon 9200 needs this initial ambient light
		// even if it is changed later (or else everything goes dark)
		O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(1.0, 1.0, 1.0));
	}


	void O3dConfigParserModule
	::createCamera(float near, float far, float fovy) {
		// Create the camera
		O3dSchema::playerCamera = O3dSchema::sceneManager->createCamera("PlayerCam");
		O3dSchema::playerCamera->setNearClipDistance(near);
		O3dSchema::playerCamera->setFarClipDistance(far);
		O3dSchema::playerCamera->setFOVy(Ogre::Radian(Ogre::Degree(fovy)));
		LogMsg("Created player camera");
	}


	void O3dConfigParserModule
	::createViewports(void) {
		// Create one viewport, entire window
		Ogre::Viewport* vp = O3dSchema::window->addViewport(O3dSchema::playerCamera);
		vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		O3dSchema::playerCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

		LogMsg("Created Ogre viewport");

	}




}

