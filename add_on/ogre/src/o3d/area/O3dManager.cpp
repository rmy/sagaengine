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
#include "sim/pos/PosComponent.hpp"
#include "comp/CompositeFactory.hpp"
#include "sim/SimEngine.hpp"
#include "client/thing/CameraComponent.hpp"
#include "util/system/RealClock.hpp"
#include "util/math/all.hpp"
#include <OgreOverlayElement.h>
#include <OgreCamera.h>
#include <OgreMaterialManager.h>
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>
#include <cstring>

using namespace se_client;
using namespace se_core;

namespace se_ogre {
	O3dManager
	::O3dManager() 
		: RootComponent(sct_RENDER), shouldStop_(false), debugOverlay_(0)
		, lastRenderClock_(0), isShowingDebugInfo_(false), isEditLightsOn_(false) {
		showDebugOverlay(false);
	}


	O3dManager
	::~O3dManager() {
		if (debugOverlay_) {
			debugOverlay_->hide();
		}
		//clear();
	}


	void O3dManager
	::clear() {
		/*
		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			O3dAreaComponent& c = static_cast<O3dAreaComponent&>(it.next());
			c.clear();
		}
		*/

	}


	void O3dManager
	::setEditLights(bool flag) {
		if(isEditLightsOn_ == flag)
			return;
		isEditLightsOn_ = flag;
		if(isEditLightsOn_) {
			ambientCache_ = O3dSchema::sceneManager->getAmbientLight();
			O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(.6f, .6f, .6f, 1));
		}
		else {
			O3dSchema::sceneManager->setAmbientLight(ambientCache_);
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
			//LogDetail(stepDelta);
			// When in doubt, show thing at nextPos.
			stepDelta = 1.0f;
		}

		AssertFatal(ClientSchema::camera, "No camera. Save or init file corrupt?");

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
		NodeComponentList::Iterator it(children_);
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
		//if(++frameCount >= 11) {
		//	SimSchema::simEngine.setGameOver(true);
		//	return false;
		//}

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
		if(!O3dSchema::playerCamera)
			return;

		// Don't try to move the camera if the camera is not in an area
		if(!ClientSchema::camera->pos().hasArea() || !ClientSchema::camera->nextPos().hasArea()) {
			return;
		}

		// Global coor
		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		static ViewPoint camera;
		ClientSchema::camera->worldViewPoint(alpha, camera);

		// Convert from Euler3 if necessary
		Quat4 face(camera.face_);

		// Feed Ogre
		if(CameraComponent::Ptr(*ClientSchema::camera)->doesSee()) {
			Ogre::Quaternion f(face.w_, face.x_, face.y_, face.z_);
			O3dSchema::playerCamera->setOrientation(f);
			O3dSchema::playerCamera->setPosition(camera.coor_.x_, camera.coor_.y_, camera.coor_.z_);
		}
		else {
			O3dSchema::playerCamera->setDirection(Ogre::Vector3(0, 1, 0));
			O3dSchema::playerCamera->setPosition(0, 10000000, 0);
		}
	}


	void O3dManager
	::showDebugOverlay(bool show) {
		if (show) {
			if (!debugOverlay_) {
				debugOverlay_ = Ogre::OverlayManager::getSingleton().getByName("SagaEngine/DebugOverlay");
				O3dSchema::window->resetStatistics();
			}

			debugOverlay_->show();
		}
		else {
			if (debugOverlay_) {
				debugOverlay_->hide();
			}
		}
		showDebugInfo(show);
		try {
			Ogre::MaterialPtr m = Ogre::MaterialManager::getSingleton().getByName("Basic/NavMesh");
			Ogre::CompareFunction cmp = show ? Ogre::CMPF_LESS_EQUAL : Ogre::CMPF_ALWAYS_FAIL;
			if(!m.isNull())
				m->setDepthFunction(cmp);
		} catch(...) {
			LogWarning("No navmesh material");
		}
	}


	void O3dManager
	::flipDebugOverlay() {
		if(!debugOverlay_) {
			showDebugOverlay(true);
			return;
		}
		showDebugOverlay(!debugOverlay_->isVisible());
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
			OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("SagaEngine/AverageFps");
			OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("SagaEngine/CurrFps");
			OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("SagaEngine/BestFps");
			OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("SagaEngine/WorstFps");

			const RenderTarget::FrameStats& stats = O3dSchema::window->getStatistics();

			guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
								+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
								 +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

			OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("SagaEngine/NumTris");
			guiTris->setCaption(tris + StringConverter::toString(static_cast<int>(stats.triangleCount)));

			static char buffer[255];
			PosComponent* a = ClientSchema::player->nextPos().area();
			sprintf(buffer, "Area: %s (%s)", a->owner()->name(), a->owner()->factory()->name()); 
			OverlayElement* guiArea = OverlayManager::getSingleton().getOverlayElement("SagaEngine/Area");
			guiArea->setCaption(buffer);

			se_core::ViewPoint& vp = ClientSchema::player->nextPos().local_;

			sprintf(buffer, "Coor: %.2f, %.2f, %.2f", vp.coor_.x_, vp.coor_.y_, vp.coor_.z_);
			OverlayElement* guiCoor = OverlayManager::getSingleton().getOverlayElement("SagaEngine/Coor");
			guiCoor->setCaption(buffer);

			sprintf(buffer, "Face: %.2f, %.2f, %.2f", vp.face_.yaw_ / (float)(BRAY_RES), vp.face_.pitch_ / (float)(BRAY_RES), vp.face_.roll_ / (float)(BRAY_RES));
			OverlayElement* guiFace = OverlayManager::getSingleton().getOverlayElement("SagaEngine/Face");
			guiFace->setCaption(buffer);

			OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("SagaEngine/DebugText");
			guiDbg->setCaption(debugText_);

			//sprintf(buffer, "Area: %s (%s) T(%.2f, %.2f, %.2f) R(%.2f, %.2f, %.2f)", a->owner()->name(), a->owner()->factory()->name(), vp.coor_.x_, vp.coor_.y_, vp.coor_.z_, vp.face_.yaw_ / (float)(BRAY_RES), vp.face_.pitch_ / (float)(BRAY_RES), vp.face_.roll_ / (float)(BRAY_RES));
			//guiDbg->setCaption(buffer);
		}
		catch(...) {
			// ignore
		}
	}

}
