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
#include "../thing/ThingMOList.hpp"
#include "O3dPre.hpp"

namespace se_ogre {
	class _SeOgreExport WorldManager 
		: public se_client::ClientListener, public Ogre::FrameListener {
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
		void setCentreAligned(bool flag) {
			isAreaGeomCentreAligned_ = flag;
		}

	protected: // Helper methods
		bool hasMesh(se_core::Thing& thing);
		void updateStats(void);
		int findArea(int id);
		void getAreaOffset(se_core::Area& area, Ogre::Vector3& dest);

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
		enum { AREA_RANGE = 3, AREA_SIDE = 2 * (AREA_RANGE + 1) };
		enum { MAX_AREAS = 3 * AREA_SIDE * AREA_SIDE * AREA_SIDE };
		struct {
			int id_;
			se_core::Area* area_;
			bool shouldKeep_;
			bool isNew_;
			Ogre::Vector3 offset_;
			Ogre::SceneNode* node_;
			ThingMOList::iterator_type firstThingMO_;
		} areas_[ MAX_AREAS ];
		int areaCount_;
		bool isAreaGeomCentreAligned_;
	};
}

#endif
