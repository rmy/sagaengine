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


#ifndef o3d_schema_O3dSchema_hpp
#define o3d_schema_O3dSchema_hpp

#include "../o3d.hpp"
#include "../area/o3d_area.hpp"
#include "../event/o3d_event.hpp"
#include "../input/o3d_input.hpp"
#include "../init/o3d_init.hpp"
#include "../thing/o3d_thing.hpp"
#include "../thing/ThingMOManager.hpp"
#include "../thing/ThingMOList.hpp"
#include "../widget/o3d_widget.hpp"
#include "O3dPre.H"


namespace se_ogre {
	namespace O3dSchema {
		extern _SeOgreExport se_ogre::ThingMOManager thingMOManager;


		extern _SeOgreExport Ogre::RaySceneQuery* raySceneQuery;
		extern _SeOgreExport ThingMOList thingMOList;
		extern _SeOgreExport Ogre::Root* root;
		extern _SeOgreExport Ogre::SceneManager* sceneManager;
		extern _SeOgreExport WorldManager* worldManager;
		extern _SeOgreExport Ogre::Camera* playerCamera;
		extern _SeOgreExport Ogre::RenderWindow* window;
		extern _SeOgreExport SpeechBubble* speechBubble;
		extern _SeOgreExport InputManager& inputManager();
		extern _SeOgreExport Console* console;
		extern _SeOgreExport RenderEventListeners& renderEventListeners();

		extern _SeOgreExport RenderEngine* renderEngine;

		extern _SeOgreExport float gameClock;
	}
}

#endif
