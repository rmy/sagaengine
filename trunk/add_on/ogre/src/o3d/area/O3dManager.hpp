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


#ifndef o3d_area_O3dManager_hpp
#define o3d_area_O3dManager_hpp

#include "../thing/o3d_thing.hpp"
#include "../thing/ThingMOList.hpp"
#include "../thing/MultiO3dThingComponent.hpp"
#include "O3dAreaComponent.hpp"
#include "O3dPre.hpp"
#include "comp/node/RootComponent.hpp"
#include <OgreFrameListener.h>
#include <OgreOverlay.h>


namespace se_ogre {
	class _SeOgreExport O3dManager 
		: public se_core::RootComponent, public Ogre::FrameListener {
	public:
		O3dManager();
		virtual ~O3dManager();
		void clear();

		// Ogre::FrameListener
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);
		void moveCamera(float stepDelta);

		// Debug overlay
		void showDebugOverlay(bool show);
		void flipDebugOverlay();

		void step(long when) {}

		void setCentreAligned(bool flag) {
			isAreaGeomCentreAligned_ = flag;
		}
		bool isAreaGeomCentreAligned_;

		bool isShowingDebugInfo() {
			return isShowingDebugInfo_;
		}

		void showDebugInfo(bool state) {
			isShowingDebugInfo_ = state;
		}


	protected: // Helper methods
		void updateStats(void);

	private:
		//
		bool isShowingDebugInfo_;
		bool shouldStop_;
		//
		Ogre::Overlay* debugOverlay_;
		//
		long lastRenderClock_;
	};

}


#endif
