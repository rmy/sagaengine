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


#include "ThingEntity.hpp"
#include "ThingMOInfo.hpp"
#include "all.hpp"
#include "O3dPre.hpp"
#include "../config/o3d_config.hpp"
#include "../schema/O3dSchema.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "o3d/thing/all.hpp"
#include "EntityUtil.hpp"


using namespace se_core;


namespace se_ogre {
	ThingEntity
	::ThingEntity(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory), entity_(0), state_(0), prevAnim_(0) {

		// Create a unique entity name
		char name[128];
		sprintf(name, "%d-%s", thing_.id(), thing_.name());

		// Create entity
		Ogre::MovableObject* mo = O3dSchema::sceneManager->createMovableObject(name, Ogre::EntityFactory::FACTORY_TYPE_NAME, &info_.params_);
		node_->attachObject(mo);
		entity_ = static_cast<Ogre::Entity*>(mo);
		entity_->setNormaliseNormals(true);
		entity_->setCastShadows(true);

		// Get and set animation associated with this thing
		short animId = anim();
		O3dAnimation* a = info_.animation(0, animId);
		if(a && !a->name_.isEmpty()) {
			float s = a->speed_;
			setAnimation(a->name_.get(), s);
		}

		// Get default material, if one
		const char* material = info_.defaultMaterial_.get();
		// Check if this movement mode has a special material
		if(a && !a->material_.isEmpty()) {
			const char* material = a->material_.get();
		}
		// If material is defined (default or special), set it
		if(material) {
			setMaterial(material);
		}
	}


	ThingEntity
	::~ThingEntity() {
		O3dSchema::sceneManager->destroyMovableObject(entity_);
		state_ = 0;
		entity_ = 0;
	}


	short ThingEntity
	::anim() {
		return thing_.pos().anim().movementMode();
	}


	void ThingEntity
	::addToStaticGeometry(const char* name) {
		///////
		Ogre::Vector3 nextPos
			(
			 CoorT::toFloat(thing_.nextPos().localCoor().x_),
			 CoorT::toFloat(thing_.nextPos().localCoor().y_),
			 CoorT::toFloat(thing_.nextPos().localCoor().z_)
			 );

		Ogre::Real scale = info_.scale_;
		// If radius scales the model
		if(info_.doScaleByRadius_) {
			// Interpolate between present radius and next radius
			scale *= CoorT::toFloat(thing_.nextPos().radius());

			// If scale has changed
			if(scale != currentScale_) {
				// Scale all children of this node
				node_->setScale(scale, scale, scale);

				// Store scale for future scale change checks
				currentScale_ = scale;
			}
		}

		static int counter = 0;
		static Ogre::StaticGeometry* sg = 0;
		if(counter == 0) {
			if(sg) {
				sg->build();
			}
			sg = O3dSchema::sceneManager->createStaticGeometry(name);
			counter = 4;
		}
		Ogre::Vector3 s(scale, scale, scale);
		sg->addEntity(entity_, nextPos, Ogre::Quaternion::IDENTITY, s);
		--counter;
	}


	void ThingEntity
	::setAnimation(const char* anim, float speed) {
		// Set the animation state
		state_ = entity_->getAnimationState(anim);

		// Enable animation
		state_->setEnabled(true);

		// Use default animation speed
		// TODO: Should probably change with the speed
		// properties of a thing
		speed_ = speed;
	}


	void ThingEntity
	::setMaterial(const char* material) {
		entity_->setMaterialName(material);
	}


	void ThingEntity
	::animate(float stepDelta, float timeSinceLastFrame) {
		// Check if movement mode (and thus animation) has changed (ie walk to stand, etc)
		const short animId = anim();

		if(prevAnim_ != animId) {
			prevAnim_ = animId;

			O3dAnimation* a = info_.animation(0, animId);
			if(a && !a->name_.isEmpty()) {
				setAnimation(a->name_.get(), a->speed_);
			}
			else {
				state_->setEnabled(false);
				state_ = 0;
			}


			// Get default material, if one
			const char* material = info_.defaultMaterial_.get();

			// Check if this movement mode has a special material
			if(a && !a->material_.isEmpty()) {
				const char* material = a->material_.get();
			}

			// If material is defined (default or special), set it
			if(material) {
				setMaterial(material);
			}
		}

		// Add passed time to the animation state
		if(state_) state_->addTime(timeSinceLastFrame * speed_);
	}


}
