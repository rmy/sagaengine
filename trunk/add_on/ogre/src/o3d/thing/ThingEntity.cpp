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
		: ThingMO(thing, info, factory), entity_(0) {

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
		for(int i = 0; i < info_.channelCount(); ++i) {
			const short animId = thing_.pos().anim(i).movementMode();
			prevAnim_[i] = -1;
			state_[i] = 0;
			setAnimation(i, thing_.pos().anim(i));
		}

		// Get default material, if one
		const char* material = info_.defaultMaterial_.get();
		/*
		// Check if this movement mode has a special material
		if(a && !a->material_.isEmpty()) {
			const char* material = a->material_.get();
		}
		*/
		// If material is defined (default or special), set it
		if(material) {
			setMaterial(material);
		}

		hasAnimation_ = true;
	}


	ThingEntity
	::~ThingEntity() {
		O3dSchema::sceneManager->destroyMovableObject(entity_);
		entity_ = 0;
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
	::setAnimation(int channel, const se_core::Anim& anim) {
		const short animId = anim.movementMode();
		if(prevAnim_[channel] != animId) {
			prevAnim_[channel] = animId;

			if(state_[channel]) {
				state_[channel]->setEnabled(false);
				state_[channel] = 0;
			}
			O3dAnimation* a = info_.animation(channel, animId);
			if(a && !a->name_.isEmpty()) {
				// Set the animation state
				state_[channel] = entity_->getAnimationState(a->name_.get());

				// Enable animation
				state_[channel]->setEnabled(true);

				// The anim speed of the mesh animation,
				// multiplied by the anim speed of the things anim channel
				const float s = a->speed_ * ScaleT::fromFloat(anim.speed());
				speed_[channel] = s;
			}
		}

	}


	void ThingEntity
	::setMaterial(const char* material) {
		entity_->setMaterialName(material);
	}


	void ThingEntity
	::animate(long when, float stepDelta, float timeSinceLastFrame) {
		// Check if movement mode (and thus animation) has changed (ie walk to stand, etc)

		for(int i = 0; i < info_.channelCount(); ++i) {
			const Anim& a = thing_.pos().anim(i);
			setAnimation(i, a);

			// Add passed time and update weight of animation
			if(state_[i]) {
				//LogMsg(i << ": " << a.movementMode());
				float pos = ScaleT::toFloat(a.startPos()) * state_[i]->getLength();
				pos += a.movementWhen(when) * (1.0f / 1024.0f) * speed_[i];
				//LogMsg(pos);
				//state_[i]->addTime(timeSinceLastFrame * speed_[i]);
				state_[i]->setTimePosition(pos);
				state_[i]->setWeight(a.weight());
			}
		}

		/*
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
		*/
	}


}
