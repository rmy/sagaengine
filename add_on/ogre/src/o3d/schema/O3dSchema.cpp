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
#include "o3d/RenderEngine.hpp"
#include "o3d/area/WorldManager.hpp"
#include "o3d/event/RenderEventListeners.hpp"
#include "o3d/thing/MeshOfThing.hpp"
#include "o3d/thing/ThingEntityList.hpp"
#include "o3d/widget/SpeechBubble.hpp"
#include "o3d/io/all.hpp"
#include "o3d/input/InputManager.hpp"
#include "o3d/input/Console.hpp"
#include "o3d/init/O3dInitHandler.hpp"

using namespace Ogre;
using namespace se_ogre;

namespace se_ogre {
	namespace O3dSchema {
		MeshOfThing meshOfThing;

		RaySceneQuery* raySceneQuery = 0;
		ThingEntityList thingEntityList(1);

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
		struct AutoInit {
			AutoInit() {
				// Auto create and register instance of this parsers
				static O3dThingParserModule o3dThingPM(se_core::IoSchema::parser());
				static O3dConfigParserModule o3dConfigPM(se_core::IoSchema::parser());
				static O3dAreaParserModule o3dAreaPM(se_core::IoSchema::parser());
				static O3dInitHandler initHandler;

				LogMsg("Registered Ogre add-on");
			}

			~AutoInit() {
				LogMsg("Cleaned up Ogre add-on");
			}
		} autoInit;
	}
}

