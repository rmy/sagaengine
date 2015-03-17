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
#include "../thing/o3d_thing.hpp"
#include "../thing/ThingMOManager.hpp"
#include "../widget/o3d_widget.hpp"
#include "util/task/util_task.hpp"
#include "O3dPre.hpp"
#include <OgreOverlaySystem.h>
#include "sim/setting/Setting.hpp"

/**
 * The se_ogre add_on module is a gluing layer between SagaEngine and the Ogre 3d enginge (http://www.ogre3d.org/).
 */
namespace se_ogre {
	/**
	 * Global variables used by the se_ogre add-on module.
	 */
	namespace O3dSchema {
		/**
		 * Manager that maintins bindings between SagaEngine things and their representation in Ogre.
		 */
		extern _SeOgreExport se_ogre::ThingMOManager thingMOManager;

		extern _SeOgreExport se_core::TaskList taskList;

		extern _SeOgreExport Ogre::RaySceneQuery* raySceneQuery;

		class TextureSetting : public se_core::Setting {
		public:
			TextureSetting() : Setting("Setting.TEXTURE_RESOLUTION", 0, 2) {
				static const char* values[] = {
					"Setting.TEXTURE_RES_HIGH",
					"Setting.TEXTURE_RES_MEDIUM",
					"Setting.TEXTURE_RES_LOW",
					0
				};
				setValueNames(values);
			}

			const char* ext() const {
				static const char* values[] = {
					".high", ".medium", ".low"
				};
				return values[ value() ];
			}
		};
		extern _SeOgreExport TextureSetting textureSetting;

		class ShadowSetting : public se_core::Setting {
		public:
			ShadowSetting() : Setting("Setting.SHADOW", 1, 1) {
				static const char* values[] = {
					"Setting.SHADOW_OFF",
					"Setting.SHADOW_ON",
					0
				};
				setValueNames(values);
			}
		};
		extern _SeOgreExport ShadowSetting shadowSetting;

		/**
		 * The Ogre::Root singleton.
		 */
		extern _SeOgreExport Ogre::Root* root;

		/**
		 * The Ogre::LogManager singleton.
		 */
		extern _SeOgreExport Ogre::LogManager* logManager;

		/**
		 * The Ogre::SceneManager singleton.
		  */
		extern _SeOgreExport Ogre::SceneManager* sceneManager;
		
		/**
		 * The manager object that controls what ogre should render.
		 */
		extern _SeOgreExport O3dManager* worldManager;

		/**
		 * Manager for input event listeners.
		 * Register the class that should recieve mouse and input events here.
		 */
		extern _SeOgreExport InputManager& inputManager();

		extern _SeOgreExport Ogre::Camera* playerCamera;
		extern _SeOgreExport Ogre::RenderWindow* window;
		extern _SeOgreExport SpeechBubble* speechBubble;
		extern _SeOgreExport Console* console;
		extern _SeOgreExport RenderEventListeners& renderEventListeners();

		extern _SeOgreExport RenderEngine* renderEngine;
		extern _SeOgreExport Ogre::OverlaySystem *overlaySystem;

		extern _SeOgreExport float gameClock;

		void _SeOgreExport reinit();

		/** Force linking of dependencies */
		void _SeOgreExport touch();
	}
}

#endif
