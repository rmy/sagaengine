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


#ifndef o3d_area_WorldManager_hpp
#define o3d_area_WorldManager_hpp

#include "../thing/o3d_thing.hpp"
#include "../thing/ThingEntityList.hpp"
#include "O3dPre.H"

namespace se_ogre {
	class WorldManager : public se_core::ClientListener, public Ogre::FrameListener {
	public:
		WorldManager();
		virtual ~WorldManager();

		void clearWorld();

		// se_core::ClientListener
		void cameraEnteredAreaEvent(se_core::Area& area);
		void cameraLeftAreaEvent(se_core::Area& area);
		//void thingEnteredCameraAreaEvent(se_core::Thing& thing);
		//void thingLeftCameraAreaEvent(se_core::Thing& thing);
		void thingEnteredActiveZoneEvent(se_core::Thing& thing);
		void thingLeftActiveZoneEvent(se_core::Thing& thing);
		void thingSwitchedActiveAreaEvent(se_core::Thing& thing);

		// Ogre::FrameListener
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);
		void moveCamera(float stepDelta);

		// Debug overlay
		void showDebugOverlay(bool show);


	protected: // Helper methods
		bool hasMesh(se_core::Thing& thing);
		void updateStats(void);
		int findArea(int id);

	private:
		//
		ThingEntity* playerEntity_;
		//
		bool shouldStop_;
		//
		Ogre::Overlay* debugOverlay_;
		//
		long lastRenderClock_;

		// 
		enum { AREA_RANGE = 2, AREA_SIDE = 2 * (AREA_RANGE + 1) };
		enum { MAX_AREAS = 3 * AREA_SIDE * AREA_SIDE * AREA_SIDE };
		struct {
			int id_;
			bool shouldKeep_;
			Ogre::SceneNode* node_;
			ThingEntityList::iterator_type firstThingEntity;
		} areas_[ MAX_AREAS ];
		int areaCount_;
	};
}

#endif
