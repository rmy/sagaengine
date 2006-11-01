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
		static int pool = 1000;
		//sprintf(name, "%d-%s", thing_.id(), thing_.name());
		sprintf(name, "%d-%s", ++pool, thing_.name());

		// Create entity
		Ogre::MovableObject* mo = O3dSchema::sceneManager->createMovableObject(name, Ogre::EntityFactory::FACTORY_TYPE_NAME, &info_.params_);
		node_->attachObject(mo);
		entity_ = static_cast<Ogre::Entity*>(mo);
		entity_->setNormaliseNormals(true);
		entity_->setCastShadows(info.isShadowCaster_);
		if(entity_->hasSkeleton()) {
			entity_->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
		}

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
			 CoorT::toFloat(thing_.nextPos().worldCoor().x_),
			 CoorT::toFloat(thing_.nextPos().worldCoor().y_),
			 CoorT::toFloat(thing_.nextPos().worldCoor().z_)
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
				//state_[channel]->setEnabled(false);
				state_[channel] = 0;
			}
			O3dAnimation* a = info_.animation(channel, animId);
			if(a && !a->name_.isEmpty()) {
				// Set the animation state
				state_[channel] = entity_->getAnimationState(a->name_.get());

				// Enable looping
				state_[channel]->setLoop(a->doLoop_);

				// The anim speed of the mesh animation,
				// multiplied by the anim speed of the things anim channel
				float s = a->speed_;
				if(s > 0) {
					s = 1 / s;
				}
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
		// Reset movement state, so that starting state is known
		for(int i = 0; i < info_.channelCount(); ++i) {
			if(state_[i]) {
				state_[i]->setEnabled(false);
				state_[i]->setWeight(0);
				state_[i]->setTimePosition(.25);
			}
		}

		for(int i = 0; i < info_.channelCount(); ++i) {
			const Anim& a = thing_.pos().anim(i);
			// Check if movement mode (and thus animation) has changed (ie walk to stand, etc)
			setAnimation(i, a);

			// Add passed time and update weight of animation
			if(state_[i]) {
				const Anim& na = thing_.nextPos().anim(i);

				float pos = ScaleT::toFloat(a.pos());
				if(a.movementMode() == na.movementMode()) {
					pos += stepDelta * (na.pos() - a.pos());
				}
				pos *= state_[i]->getLength() * speed_[i];
				// If same state are in more channels, interpolate between their positions
				Assert(!state_[i]->getEnabled() && "Using the same animation twice at once");
				if(a.weight() > 0) {
					//state_[i]->setTimePosition(pos);
					state_[i]->setTimePosition(pos);
					float w = a.weight();
					if(a.movementMode() == na.movementMode()) {
						w += stepDelta * (na.weight() - a.weight());
					}
					state_[i]->setWeight(w);
					state_[i]->setEnabled(true);
					//LogMsg(thing_.name() << ": " << state_[i]->getAnimationName().c_str() << ": " << w << " - " << stepDelta);
				}
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
