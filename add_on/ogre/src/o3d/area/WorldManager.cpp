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
#include "WorldManager.hpp"
#include "all.hpp"
#include "../config/o3d_config.hpp"
#include "../input/InputHandler.hpp"
#include "../input/InputManager.hpp"
#include "../event/RenderEventListeners.hpp"
#include "../schema/all.hpp"
#include "../thing/ThingMO.hpp"
#include "../thing/ThingMOList.hpp"
#include "../thing/ThingMOManager.hpp"
#include <cstring>

using namespace se_client;
using namespace se_core;

namespace se_ogre {
	WorldManager
	::WorldManager() 
		: playerEntity_(0), shouldStop_(false), debugOverlay_(0)
		, lastRenderClock_(0), areaCount_(0), isAreaGeomCentreAligned_(false) {
		showDebugOverlay(false);
	}


	WorldManager
	::~WorldManager() {
		clearWorld();
	}


	void WorldManager
	::clearWorld() {
		for(int i = 0; i < areaCount_; ++i) {
			ThingMOList::iterator_type it = areas_[i].firstThingMO_;
			while(it != ThingMOList::NULL_NODE) {
				ThingMO* te = O3dSchema::thingMOList.next(it);
				O3dSchema::thingMOManager.release(te);
			}
			O3dSchema::thingMOList.removeChain(areas_[i].firstThingMO_);
		}
	}


	int WorldManager
	::findArea(int id) {
		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[i].id_ == id)
				return i;
		}
		return -1;
	}


	void WorldManager
	::getAreaOffset(se_core::Area& area, Ogre::Vector3& dest) {
		dest.x = area.pos().localCoor().x_;
		dest.y = area.pos().localCoor().y_;
		dest.z = area.pos().localCoor().z_;
		
		if(isAreaGeomCentreAligned_) {
			dest.x += area.width() / 2.0f;
			dest.z += area.height() / 2.0f;
		}
	}


	void WorldManager
	::cameraEnteredAreaEvent(se_core::Area& area) {
		for(int i = 0; i < areaCount_; ++i) {
			areas_[i].shouldKeep_ = false;
		}

		// Build terrain
		try {
			const char* areaType = (area.factory() != 0) ? area.factory()->name() : area.name();
			O3dSchema::sceneManager->setWorldGeometry(Ogre::String(areaType));
		}
		catch(...) {
			LogMsg("Couldn't load geometry for area: " << area.name());

			for(short relZ = -AREA_RANGE; relZ <= AREA_RANGE; ++relZ) {
				for(short relX = -AREA_RANGE; relX <= AREA_RANGE; ++relX) {
					Area* a = area.neighbour(relX, 0, relZ);
					if(!a) continue;
					int index = findArea(a->id());
					if(index >= 0) {
						areas_[ index ].shouldKeep_ = true;
						continue;
					}

					const char* areaType = (a->factory() != 0) ? a->factory()->name() : a->name();

					Ogre::String type(areaType);
					Ogre::String name(a->name());

					try {
						static int pool = 0;
						static char buffer[128];
						sprintf(buffer, "%s.%d", a->name(), pool++);
						Ogre::Entity* entity = O3dSchema::sceneManager->createEntity(buffer, type + ".mesh");
						entity->setNormaliseNormals(true);
						entity->setCastShadows(false);

						index = areaCount_++;
						areas_[ index ].area_ = a;
						areas_[ index ].shouldKeep_ = true;
						areas_[ index ].isNew_ = true;
						areas_[ index ].firstThingMO_ = ThingMOList::NULL_NODE;
						areas_[ index ].id_ = a->id();
						areas_[ index ].node_ = O3dSchema::sceneManager->createSceneNode();

						getAreaOffset(*a, areas_[ index ].offset_);
						areas_[ index ].node_->attachObject(entity);
						areas_[ index ].node_->setPosition(areas_[ index ].offset_);

						O3dSchema::sceneManager->getRootSceneNode()->addChild(areas_[ index ].node_);

					}
					catch(...) {
						LogMsg("Couldn't load area mesh " << areaType << ".mesh for " << a->name() );
					}
				}
			}
		}

		// Throw away areas that shouldn't be kept
		for(int i = 0; i < areaCount_; ++i) {
			if(!areas_[i].shouldKeep_) {
				// Remove area from scene graph
				O3dSchema::sceneManager->getRootSceneNode()->removeAndDestroyChild(areas_[i].node_->getName());

				// Remove things
				ThingMOList::iterator_type it = areas_[i].firstThingMO_;
				while(it != ThingMOList::NULL_NODE) {
					ThingMO* te = O3dSchema::thingMOList.next(it);
					O3dSchema::thingMOManager.release(te);
				}
				O3dSchema::thingMOList.removeChain(areas_[i].firstThingMO_);

				// Move last area in array to here
				areas_[i] = areas_[ --areaCount_];
				// Do this index again (as it now contains a new area)
				--i;
			}
		}


		// Add area things in new areas
		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[i].isNew_) {
				Area* a = areas_[i].area_;
				LogMsg(a->name());
				// Add things
				se_core::SimObjectIterator nit(a->reportingThings());
				while(nit.hasNext()) {
					WasHere();
					Thing& thing = nit.nextThing();
					if(!hasMesh(thing)) {
						continue;
					}

					ThingMO* te = O3dSchema::thingMOManager.create(thing);
					O3dSchema::thingMOList.add(*te, areas_[ i ].firstThingMO_);
				}

				areas_[i].isNew_ = false;
			}
		}

	}


	void WorldManager
	::cameraLeftAreaEvent(se_core::Area& area) {
		/*
		// Clear thinglist
		ThingMOList::iterator_type it = areas_[index].firstThingMO;
		while(it != ThingMOList::NULL_NODE) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			ThingMO* te = O3dSchema::thingMOManager.release(te);
		}
		O3dSchema::thingMOList.removeChain(O3dSchema::firstThingMO);
		*/
	}


	void WorldManager
	::thingEnteredActiveZoneEvent(se_core::Thing& thing) {
		LogMsg("Thing entered active zone: " << thing.name());
		if(!hasMesh(thing)) {
			return;
		}

		// 
		int index = findArea(thing.nextPos().area()->id());
		if(index < 0) return;

		// Add thing
		ThingMO* te = O3dSchema::thingMOManager.create(thing);
		O3dSchema::thingMOList.add(*te, areas_[index].firstThingMO_);
	}


	void WorldManager
	::thingLeftActiveZoneEvent(se_core::Thing& thing) {
		LogMsg("Thing left active zone: " << thing.name());
		if(!hasMesh(thing)) {
			return;
		}

		// Remove thing
		int index = findArea(thing.pos().area()->id());
		if(index < 0) return;

		ThingMOList::iterator_type it = areas_[index].firstThingMO_;
		while(it != ThingMOList::NULL_NODE) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			if(te->hasThing(thing)) {
				O3dSchema::thingMOList.remove(*te, areas_[index].firstThingMO_);
				O3dSchema::thingMOManager.release(te);
				break;
			}
		}

	}


	void WorldManager
	::thingSwitchedActiveAreaEvent(se_core::Thing& thing) {
		if(!hasMesh(thing)) {
			return;
		}

		// Remove thing
		int index = findArea(thing.pos().area()->id());
		if(index < 0) return;
		int nextIndex = -1;
		if(thing.nextPos().hasArea())
			nextIndex = findArea(thing.nextPos().area()->id());

		ThingMOList::iterator_type it = areas_[index].firstThingMO_;
		while(it != ThingMOList::NULL_NODE) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			if(te->hasThing(thing)) {
				O3dSchema::thingMOList.remove(*te, areas_[index].firstThingMO_);
				if(nextIndex >= 0) {
					// Add to new area if it is visible
					O3dSchema::thingMOList.add(*te, areas_[nextIndex].firstThingMO_);
				}
				else {
					// ... or destroy
					O3dSchema::thingMOManager.release(te);
				}
				break;
			}
		}
	}


	bool WorldManager
	::hasMesh(se_core::Thing& thing) {
		// Find which mesh this thing should use
		short index = O3dSchema::thingMOManager.infoIndex(thing.name());

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
		for(int i = 0; i < areaCount_; ++i) {
			ThingMOList::iterator_type it = areas_[i].firstThingMO_;
			while(it != ThingMOList::NULL_NODE) {
				ThingMO* te = O3dSchema::thingMOList.next(it);
				te->move(renderClock, stepDelta, timeSinceLastFrame);
			}
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
