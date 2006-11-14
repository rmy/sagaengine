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
#include "O3dManager.hpp"
#include "all.hpp"
#include "../config/o3d_config.hpp"
#include "../input/InputHandler.hpp"
#include "../input/InputManager.hpp"
#include "../event/RenderEventListeners.hpp"
#include "../schema/all.hpp"
#include "../thing/O3dThingComponent.hpp"
#include "../thing/MultiO3dThingComponent.hpp"
#include "../thing/ThingMO.hpp"
#include "../thing/ThingMOInfo.hpp"
#include "../thing/ThingMOList.hpp"
#include "../thing/ThingMOManager.hpp"
#include <cstring>

using namespace se_client;
using namespace se_core;

namespace se_ogre {
	O3dManager
	::O3dManager() 
		: SimComponentManager(sct_RENDER), shouldStop_(false), debugOverlay_(0)
		, lastRenderClock_(0) {
		showDebugOverlay(false);
	}


	O3dManager
	::~O3dManager() {
		//clear();
	}


	void O3dManager
	::clear() {
		MultiSimNodeComponent::TreeIterator it(children_);
		while(it.hasNext()) {
			O3dAreaComponent& c = static_cast<O3dAreaComponent&>(it.next());
			c.clear();
		}

	}



	bool O3dManager
	::frameStarted(const Ogre::FrameEvent& evt) {
		// Any in game events caused the game to end?
		if(SimSchema::simEngine.isGameOver()) {
			// Will cause renderloop to end
			return false;
		}

		long renderClock = SimSchema::realClock->millis();

		// We are inbetwen AI steps. Calculate exactly where.
		// stepDelta 0.0f means we are at Thing::pos, 1.0f at nextPos.
		float stepDelta = (renderClock - SimSchema::simEngine.when())
			/ static_cast<float>(TIMESTEP_INTERVAL);
		if(stepDelta < 0.0f || stepDelta > 1.0f) {
			LogMsg(stepDelta);
			// When in doubt, show thing at nextPos.
			stepDelta = 1.0f;
		}

		// Eliminate jumps caused by interpolation when teleporting between levels.
		if(!ClientSchema::camera->pos().isKeyFramePath(ClientSchema::camera->nextPos())) {
			stepDelta = 1.0f;
		}
		// Interpolate new camera position
		moveCamera(stepDelta);

		// Cast UI update events
		float rc = renderClock / 1000.0f;
		O3dSchema::renderEventListeners().castUpdateLights(rc);
		O3dSchema::renderEventListeners().castUpdateMaterials(rc);
		O3dSchema::renderEventListeners().castUpdateImpostors(rc);

		//
		float timeSinceLastFrame = (renderClock - lastRenderClock_) / 1000.0f;

		// Interpolate world positions at this stepDelta for all Things in scene.
		MultiSimNodeComponent::TreeIterator it(children_);
		while(it.hasNext()) {
			O3dAreaComponent& c = static_cast<O3dAreaComponent&>(it.next());
			c.move(renderClock, stepDelta, timeSinceLastFrame);
		}

		// Store time
		lastRenderClock_ = renderClock;

		// true continues renderloop
		return true;
	}


	bool O3dManager
	::frameEnded(const Ogre::FrameEvent& evt) {
		// Any in game events caused the game to end?
		if(SimSchema::simEngine.isGameOver()) {
			// Will cause renderloop to end
			return false;
		}

		// Update debug info
		if(debugOverlay_) updateStats();

		// Auto quit for debug use
		//static int frameCount = 0;
		//if(++frameCount >= 2)
		//	return false;

		// Renderloop ends on false
		return !shouldStop_;
	}


	void O3dManager
	::moveCamera(float stepDelta) {
		if(O3dSchema::inputManager().active()) {
			if(O3dSchema::inputManager().active()->moveCamera(stepDelta)) {
				return;
			}
		}

		// Don't try to move the camera if the camera is not in an area
		if(!ClientSchema::camera->pos().hasArea()) {
			return;
		}

		// Global coor
		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		static ViewPoint camera;
		ClientSchema::camera->worldViewPoint(alpha, camera);

		// Convert from Euler3 if necessary
		Quat4 face(camera.face_);

		// Feed Ogre
		Ogre::Quaternion f(face.w_, face.x_, face.y_, face.z_);
		O3dSchema::playerCamera->setOrientation(f);
		O3dSchema::playerCamera->setPosition(camera.coor_.x_, camera.coor_.y_, camera.coor_.z_);
	}


	void O3dManager
	::showDebugOverlay(bool show) {
		if (show) {
			if (!debugOverlay_) {
				debugOverlay_ = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
			}

			debugOverlay_->show();
		}
		else {
			if (debugOverlay_) {
				debugOverlay_->hide();
			}
		}
	}


	void O3dManager
	::updateStats(void) {
		using namespace Ogre;


		static Ogre::String currFps = "Current FPS: ";
		static Ogre::String avgFps = "Average FPS: ";
		static Ogre::String bestFps = "Best FPS: ";
		static Ogre::String worstFps = "Worst FPS: ";
		static Ogre::String tris = "Triangle Count: ";

		// update stats when necessary
		try {
			OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
			OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
			OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
			OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

			const RenderTarget::FrameStats& stats = O3dSchema::window->getStatistics();

			guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
								+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
								 +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

			OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
			guiTris->setCaption(tris + StringConverter::toString(static_cast<int>(stats.triangleCount)));

			OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
			//guiDbg->setCaption(O3dSchema::window->getDebugText());
			static char buffer[255];
			se_core::ViewPoint& vp = ClientSchema::player->nextPos().local_;
			Area* a = ClientSchema::player->nextPos().area();
			sprintf(buffer, "Area: %s (%s) T(%.2f, %.2f, %.2f) R(%.2f, %.2f, %.2f)", a->name(), a->factory()->name(), vp.coor_.x_, vp.coor_.y_, vp.coor_.z_, vp.face_.yaw_ / (float)(BRAY_RES), vp.face_.pitch_ / (float)(BRAY_RES), vp.face_.roll_ / (float)(BRAY_RES));
			guiDbg->setCaption(buffer);
		}
		catch(...) {
			// ignore
		}
	}

}
