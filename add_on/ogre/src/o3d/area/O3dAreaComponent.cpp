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
#include "O3dAreaComponent.hpp"
#include "WorldManager.hpp"
#include "../schema/O3dSchema.hpp"
#include "../thing/ThingMO.hpp"
#include "../thing/ThingMOInfo.hpp"
#include "../thing/MultiO3dThingComponent.hpp"

using namespace se_core;
using namespace se_client;

namespace se_ogre {

	O3dAreaComponent
	::O3dAreaComponent(SimComposite* owner)
		: SimComponent(sct_RENDER, owner), isVisible_(false), isInitialized_(false) {

		areaOffset(offset_);

		// Create areas node, and add it to scene manager
		node_ = O3dSchema::sceneManager->createSceneNode();
		node_->setPosition(offset_);
	}


	O3dAreaComponent
	::~O3dAreaComponent() {
		setVisible(false);

		node_->removeAndDestroyAllChildren();
		O3dSchema::sceneManager->destroySceneNode(node_->getName());
		node_ = 0;
	}


	void O3dAreaComponent
	::init() {
		if(isInitialized_)
			return;

		isInitialized_ = true;

		LogMsg(owner()->name() << ": " << offset_.x << ", " << offset_.y << ", " << offset_.z);

		staticGeometry_ = compileStaticGeometry();
		initThings();

		node_->_updateBounds();
	}


	void O3dAreaComponent
	::initThings() {
		// Add things
		Area* a = toArea();
		se_core::SimObjectIterator nit(a->reportingThings());
		while(nit.hasNext()) {
			Thing& thing = nit.nextThing();
			if(!WorldManager::hasMesh(thing)) {
				continue;
			}

			O3dThingComponent* tc = O3dSchema::thingMOManager.create(thing);
			tc->setParentNode(node_);
			tc->setAreaList(children_);
		}

	}

	void O3dAreaComponent
	::addChild(O3dThingComponent& c) {
		children_.add(c);
	}


	void O3dAreaComponent
	::removeChild(O3dThingComponent& c) {
		children_.remove(c);
	}


	void O3dAreaComponent
	::setActive(bool state) {
		LogMsg(owner()->name() << ": " << state);
	}


	void O3dAreaComponent
	::setVisible(bool state) {
		if(isVisible_ == state)
			return;

		isVisible_ = state;
		if(isVisible_) {
			LogMsg("Now visible");
			init();
			initThings();
			O3dSchema::sceneManager->getRootSceneNode()->addChild(node_);
			staticGeometry_->setVisible(true);
		}
		else {
			O3dSchema::sceneManager->getRootSceneNode()->removeChild(node_);
			staticGeometry_->setVisible(true);
		}
	}


	void O3dAreaComponent
	::areaOffset(Ogre::Vector3& dest) {
		dest.x = owner()->pos().worldCoor().x_;
		dest.y = owner()->pos().worldCoor().y_;
		dest.z = owner()->pos().worldCoor().z_;
		// TODO:
		//if(isAreaGeomCentreAligned_) {
			dest.x += toArea()->width() / 2.0f;
			dest.z += toArea()->height() / 2.0f;
		//}
	}


	Ogre::StaticGeometry* O3dAreaComponent
	::compileStaticGeometry() {
		Area* a = toArea();
		Ogre::StaticGeometry* sg = O3dSchema::sceneManager->createStaticGeometry(owner()->name());
		Ogre::Entity* entity;

		sg->setOrigin(offset_);
		
		// Area geometry
		const char* areaType = (a->factory() != 0) ? a->factory()->name() : a->name();
		Ogre::String type(areaType);
		if(O3dSchema::sceneManager->hasEntity(type)) {
			entity = O3dSchema::sceneManager->getEntity(type);
		}
		else {
			entity = O3dSchema::sceneManager->createEntity(type, type + ".mesh");
			entity->setNormaliseNormals(true);
		}
		sg->addEntity(entity, offset_, Ogre::Quaternion::IDENTITY, Ogre::Vector3(1, 1, 1));

		// Add static things
		se_core::SimObjectIterator nit(a->reportingThings());
		while(nit.hasNext()) {
			Thing& thing = nit.nextThing();
			if(!WorldManager::hasStaticGeometry(thing)) {
				continue;
			}

			// Create entity
			const ThingMOInfo* info = O3dSchema::thingMOManager.info(thing.name());
			Ogre::Entity* entity;
			if(O3dSchema::sceneManager->hasEntity(thing.name())) {
				entity = O3dSchema::sceneManager->getEntity(thing.name());
			}
			else {
				Ogre::MovableObject* mo
					= O3dSchema::sceneManager->createMovableObject(thing.name(), Ogre::EntityFactory::FACTORY_TYPE_NAME, &info->params_);
				entity = static_cast<Ogre::Entity*>(mo);
				entity->setNormaliseNormals(true);
			}


			ViewPoint& nextPos = thing.nextPos().world_;
			Ogre::Vector3 pos(
				CoorT::toFloat(nextPos.coor_.x_),
				CoorT::toFloat(nextPos.coor_.y_),
				CoorT::toFloat(nextPos.coor_.z_)
				);

			Quat4 face(nextPos.face_);
			Ogre::Quaternion rot(
				QuatT::toFloat(face.w_),
				QuatT::toFloat(face.x_),
				QuatT::toFloat(face.y_),
				QuatT::toFloat(face.z_)
				);

			Ogre::Real scale = info->scale_;
			// If radius scales the model
			if(info->doScaleByRadius_) {
				// Interpolate between present radius and next radius
				scale *= CoorT::toFloat(thing.nextPos().radius());
			}

			Ogre::Vector3 s(scale, scale, scale);
			sg->addEntity(entity, pos, rot, s);
		}

		sg->setCastShadows(false);
		sg->setRegionDimensions(Ogre::Vector3(64, 64, 64));
		sg->setRenderingDistance(256);
		sg->setRenderQueueGroup(Ogre::RENDER_QUEUE_WORLD_GEOMETRY_1);
		sg->setVisible(true);
		sg->build();

		return sg;
	}


	void O3dAreaComponent
	::move(long when, float stepDelta, float timeSinceLastFrame) {
		MultiO3dThingComponent::Iterator it(children_);
		while(it.hasNext()) {
			O3dThingComponent* tc = &it.next();
			tc->move(when, stepDelta, timeSinceLastFrame);
		}
	}

}