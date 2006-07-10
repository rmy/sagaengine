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
#include "WorldManager.hpp"
#include "all.hpp"
#include "../config/o3d_config.hpp"
#include "../input/InputHandler.hpp"
#include "../input/InputManager.hpp"
#include "../event/RenderEventListeners.hpp"
#include "../schema/all.hpp"
#include "../thing/ThingEntity.hpp"
#include "../thing/ThingEntityList.hpp"
#include <cstring>

using namespace se_core;


namespace se_ogre {
	WorldManager
	::WorldManager() : playerEntity_(0), shouldStop_(false), debugOverlay_(0), lastRenderClock_(0) {
		showDebugOverlay(false);
	}


	WorldManager
	::~WorldManager() {
		clearWorld();
	}


	void WorldManager
	::clearWorld() {
		ClientSchema::clientListeners.removeListener(*this);
		ThingEntityList::iterator_type it = O3dSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = O3dSchema::thingEntityList.next(it);
			delete te;
		}
		O3dSchema::thingEntityList.removeChain(O3dSchema::firstThingEntity);
	}


	void WorldManager
	::cameraEnteredAreaEvent(se_core::Area& area) {
		// Build terrain
		try {
			O3dSchema::sceneManager->setWorldGeometry(Ogre::String(area.name()));
		}
		catch(...) {
			LogMsg("Couldn't load geometry for area: " << area.name());
			try {
				Ogre::String name(area.name());
				Ogre::Entity* entity = O3dSchema::sceneManager->createEntity(name, name + ".mesh");
				entity->setNormaliseNormals(true);
				entity->setCastShadows(false);
				O3dSchema::sceneManager->getRootSceneNode()->attachObject(entity);
			}
			catch(...) {
				LogMsg("Couldn't load area mesh either: " << area.name() << ".mesh");
			}
		}


		for(short relZ = -3; relZ <= 3; ++relZ) {
			for(short relX = -3; relX <= 3; ++relX) {
				Area* a = area.getNeighbour(relX, relZ);
				if(!a) continue;

				// Add things
				se_core::ThingIterator nit(a->allThings());
				while(nit.hasNext()) {
					Thing& thing = nit.next();
					LogMsg(thing.name());
					if(!hasThingEntity(thing)) {
						continue;
					}

					LogMsg(thing.name());
					ThingEntity* te = new ThingEntity(thing);
					O3dSchema::thingEntityList.add(*te, O3dSchema::firstThingEntity);
				}
			}
		}
	}


	void WorldManager
	::cameraLeftAreaEvent(se_core::Area& area) {
		//O3dSchema::sceneManager->clearScene();
		//O3dSchema::sceneManager->getSceneNode("AreaSceneNode")->removeAndDestroyAllChildren();
		// Clear thinglist
		ThingEntityList::iterator_type it = O3dSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = O3dSchema::thingEntityList.next(it);
			delete te;
		}
		O3dSchema::thingEntityList.removeChain(O3dSchema::firstThingEntity);
	}


	void WorldManager
	::thingEnteredCameraAreaEvent(se_core::Thing& thing) {
		//LogMsg(thing.name());
		if(!hasThingEntity(thing)) {
			return;
		}
		//LogMsg(thing.name());

		// Add thing
		ThingEntity* te = new ThingEntity(thing);
		O3dSchema::thingEntityList.add(*te, O3dSchema::firstThingEntity);
	}


	void WorldManager
	::thingLeftCameraAreaEvent(se_core::Thing& thing) {
		if(!hasThingEntity(thing)) {
			return;
		}

		// Remove thing
		ThingEntityList::iterator_type it = O3dSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = O3dSchema::thingEntityList.next(it);
			if(te->hasThing(thing)) {
				O3dSchema::thingEntityList.remove(*te, O3dSchema::firstThingEntity);
				delete te;
				break;
			}
		}
	}


	bool WorldManager
	::hasThingEntity(se_core::Thing& thing) {
		// Find which mesh this thing should use
		short index = O3dSchema::meshOfThing.index(thing.name());
		// Did find one?
		return index >= 0;
	}



	bool WorldManager
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
			/ static_cast<float>(se_core::TIMESTEP_INTERVAL);
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
		ThingEntityList::iterator_type it = O3dSchema::firstThingEntity;
		while(it != ThingEntityList::NULL_NODE) {
			ThingEntity* te = O3dSchema::thingEntityList.next(it);
			te->move(stepDelta, timeSinceLastFrame);
		}

		// Store time
		lastRenderClock_ = renderClock;

		// true continues renderloop
		return true;
	}


	bool WorldManager
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


	void WorldManager
	::moveCamera(float stepDelta) {
		if(O3dSchema::inputManager().active()) {
			if(O3dSchema::inputManager().active()->moveCamera(stepDelta)) {
				return;
			}
		}

		// Don't try to move the camera it the camera is not in an area
		if(!ClientSchema::camera->pos().hasArea())
			return;


		// Interpolate current frame position
		Coor w;
		ClientSchema::camera->worldCoor(stepDelta, w);

		// Convert to ogre format
		Ogre::Vector3 pos
			(
			 CoorT::toFloat(w.x_),
			 CoorT::toFloat(w.y_),
			 CoorT::toFloat(w.z_)
			 );

		O3dSchema::playerCamera->setPosition(pos.x, pos.y + 10.0f, pos.z + 10.0f);
		O3dSchema::playerCamera->lookAt(pos.x, pos.y, pos.z);
	}


	void WorldManager
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


	void WorldManager
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
			guiDbg->setCaption(O3dSchema::window->getDebugText());
		}
		catch(...) {
			// ignore
		}
	}

}
