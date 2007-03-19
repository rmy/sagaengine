/*
SagaEngine library
Cpyright (c) 2002-2006 Skalden Studio AS

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
#include "O3dManager.hpp"
#include "../schema/O3dSchema.hpp"
#include "../thing/ThingMO.hpp"
#include "../thing/ThingMOInfo.hpp"
#include "../thing/MultiO3dThingComponent.hpp"

using namespace se_core;
using namespace se_client;

namespace se_ogre {

	O3dAreaComponent
	::O3dAreaComponent(Composite* owner)
		: O3dNodeComponent(sct_RENDER, owner), Task(0, 32), isVisible_(false), isInitialized_(false) {
			setPriority(0);
			setWeight(0);
			O3dSchema::taskList.add(*this);
	}


	O3dAreaComponent
	::~O3dAreaComponent() {
		O3dSchema::taskList.remove(*this);
		clear();
	}


	void O3dAreaComponent
	::init() {
		if(isInitialized_)
			return;
		isInitialized_ = true;

		areaOffset(offset_);

		// Create areas node, and add it to scene manager
		node_ = O3dSchema::sceneManager->createSceneNode();
		node_->setPosition(offset_);

		LogMsg(owner()->name() << ": " << offset_.x << ", " << offset_.y << ", " << offset_.z);
		staticGeometry_ = compileStaticGeometry();
	}


	void O3dAreaComponent
	::clear() {
		if(!isInitialized_)
			return;
		isInitialized_ = false;

		setVisible(false);
	}


	void O3dAreaComponent
	::perform() {
		init();
		LogMsg(owner()->name());
		if(!isActive())
			return;
		node_->_updateBounds();
		setVisible(true);
	}


	void O3dAreaComponent
	::setActive(bool state) {
		LogMsg(owner()->name() << ": " << state);
		if(state) {
			if(ClientSchema::player->nextPos().area() == PosComponent::get(*this)) {
				setPriority(0);
				setWeight(0);
			}
			else {
				setPriority(2);
				setWeight(isInitialized_ ? 16 : 64);
			}
			O3dSchema::taskList.add(*this);
		}
		else {
			O3dSchema::taskList.remove(*this);
			setVisible(false);
		}
	}


	void O3dAreaComponent
	::setVisible(bool state) {
		if(isVisible_ == state)
			return;

		isVisible_ = state;
		if(isVisible_) {
			LogMsg("Now visible");
			init();
			O3dSchema::sceneManager->getRootSceneNode()->addChild(node_);
			staticGeometry_->setVisible(true);
		}
		else {
			O3dSchema::sceneManager->getRootSceneNode()->removeChild(node_);
			staticGeometry_->setVisible(false);
		}
	}


	void O3dAreaComponent
	::areaOffset(Ogre::Vector3& dest) {
		const Point3& c = PosComponent::Ptr(*this)->pos().worldCoor();
		dest.x = c.x_;
		dest.y = c.y_;
		dest.z = c.z_;

		if(O3dSchema::worldManager->isAreaGeomCentreAligned_) {
			dest.x += toArea()->width() / 2.0f;
			dest.z += toArea()->height() / 2.0f;
		}
	}


	bool O3dAreaComponent
	::hasStaticGeometry(se_core::Composite& thing) {
		// Find which mesh this thing should use
		short index = O3dSchema::thingMOManager.infoIndex(thing.name());
		if(index < 0)
			return false;

		return O3dSchema::thingMOManager.info(index)->isStatic_;
	}



	Ogre::StaticGeometry* O3dAreaComponent
	::compileStaticGeometry() {
		Area* a = toArea();
		Ogre::StaticGeometry* sg = O3dSchema::sceneManager->createStaticGeometry(owner()->name());

		Ogre::Entity* entity;

		sg->setOrigin(offset_);
		
		// Area geometry
		const char* areaType = (a->factory() != 0) ? a->factory()->name() : a->name();
		//
		Ogre::String type(areaType);
		if(O3dSchema::sceneManager->hasEntity(type)) {
			entity = O3dSchema::sceneManager->getEntity(type);
		}
		else {
			entity = O3dSchema::sceneManager->createEntity(type, type + ".mesh");
			entity->setNormaliseNormals(true);
		}
		sg->addEntity(entity, offset_, Ogre::Quaternion::IDENTITY, Ogre::Vector3(1, 1, 1));

#ifdef SE_INTERNAL
		type += "_navMesh";
		if(O3dSchema::sceneManager->hasEntity(type)) {
			entity = O3dSchema::sceneManager->getEntity(type);
		}
		else {
			entity = O3dSchema::sceneManager->createEntity(type, type + ".mesh");
			entity->setNormaliseNormals(true);
		}
		entity->setMaterialName("Basic/NavMesh");
		sg->addEntity(entity, offset_, Ogre::Quaternion::IDENTITY, Ogre::Vector3(1, 1, 1));
#endif

		// Add static things
		CompositeList::Iterator it(owner()->children());
		while(it.hasNext()) {
			Composite& thing = it.next();

			if(!hasStaticGeometry(thing)) {
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


			PosComponent* p = PosComponent::get(thing);
			Assert(p);
			ViewPoint& nextPos = p->nextPos().world_;
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
				scale *= CoorT::toFloat(p->nextPos().radius());
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
		MultiSimNodeComponent::Iterator it(children_);
		while(it.hasNext()) {
			O3dThingComponent* tc = static_cast<O3dThingComponent*>(&it.next());
			tc->move(when, stepDelta, timeSinceLastFrame);
		}
	}

}
