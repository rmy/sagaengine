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
#include "ThingEntity.hpp"
#include "all.hpp"
#include "../config/o3d_config.hpp"
#include "../schema/O3dSchema.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "o3d/thing/all.hpp"
#include "EntityUtil.hpp"


using namespace se_core;


namespace se_ogre {
	ThingEntity
	::ThingEntity(Thing& thing)
			: node_(0), entity_(0), billboard_(0), movableObject_(0)
			  , state_(0), speed_(0), doScaleByRadius_(false), currentScale_(0)
			  , scale_(WORLD_SCALE), meshOut_(20.0), billboardIn_(0)
			  , thing_(&thing), prevAnim_(0), isEntityVisible_(false) {

		// Create things node, and add it to scene manager
		node_ = O3dSchema::sceneManager->createSceneNode();

		// Find which mesh this thing should use
		short index = O3dSchema::meshOfThing.index(thing_->name());
		Assert(index >= 0);
		const char* factory = O3dSchema::meshOfThing.factory(index);
		Ogre::NameValuePairList* params = O3dSchema::meshOfThing.params(index);

		// Get the scale of the mesh when used for this thing
		scale_ = O3dSchema::meshOfThing.scale(index);
		meshOut_ = O3dSchema::meshOfThing.meshOut(index);
		billboardIn_ = O3dSchema::meshOfThing.billboardIn(index);

		// Should thing be scaled by radius?
		doScaleByRadius_ = O3dSchema::meshOfThing.doScaleByRadius(index);

		// Create a unique entity name
		char name[128];
		sprintf(name, "%d-%s", thing_->id(), thing_->name());

		// Create and setentity
		setEntity(name, factory, params, scale_);

		// Get and set animation associated with this thing
		short animId = anim();
		const char* animation = O3dSchema::meshOfThing.animation(index, animId);
		if(animation) {
			float s = O3dSchema::meshOfThing.animationSpeed(index, animId);
			setAnimation(animation, s);
		}

		// Check if this movment mode has a special material
		const char* material = O3dSchema::meshOfThing.material(index, anim());

		// If not, use default material
		if(!material) {
			const char* material = O3dSchema::meshOfThing.defaultMaterial(index);
		}

		// If material is defined (default or special), set it
		if(material) {
			setMaterial(material);
		}

		//if(entity_) {
		//	tempMaterial_ = new EntityMaterial(entity_, entity_->getName().c_str());
		//}
	}


	ThingEntity
	::~ThingEntity() {
		if(node_) {
			setEntityVisible(false);
			if(entity_) {
				O3dSchema::sceneManager->destroyMovableObject(entity_);
				state_ = 0;
				entity_ = 0;
			}
			else if(movableObject_) {
				O3dSchema::sceneManager->destroyMovableObject(movableObject_);
				movableObject_ = 0;
			}
			if(billboard_) {
				const char* bbsName = thing_->pos().area()->name();
				Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);
				bbs->removeBillboard(billboard_);
				billboard_ = 0;
			}

			node_->removeAndDestroyAllChildren();
			O3dSchema::sceneManager->destroySceneNode(node_->getName());

			node_ = 0;
		}
	}


	short ThingEntity
	::anim() {
		return thing_->anim().movementMode();
	}


	bool ThingEntity
	::endsWith(const char* s, const char* postfix) {
		size_t plen = strlen(postfix);
		size_t slen = strlen(s);
		if(slen < plen) return false;
		for(unsigned int i = 0; i < plen; ++i) {
			if(s[slen - plen + i] != postfix[i])
				return false;
		}
		return true;
	}


	void ThingEntity
	::addToStaticGeometry(const char* name) {
		///////
		Ogre::Vector3 nextPos
			(
			 CoorT::toFloat(thing_->nextPos().coor().x_),
			 CoorT::toFloat(thing_->nextPos().coor().y_),
			 CoorT::toFloat(thing_->nextPos().coor().z_)
			 );

		Ogre::Real scale = scale_;
		// If radius scales the model
		if(doScaleByRadius_) {
			// Interpolate between present radius and next radius
			scale = CoorT::toFloat(thing_->nextPos().radius());
			scale *= scale_;

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
	::setEntity(const char* name, const char* factory, Ogre::NameValuePairList* params, float scale) {
		// Set the mesh
		movableObject_ = O3dSchema::sceneManager->createMovableObject(name, factory, params);
		node_->attachObject(movableObject_);

		if(strcmp(Ogre::EntityFactory::FACTORY_TYPE_NAME.c_str(), factory) == 0) {
			entity_ = static_cast<Ogre::Entity*>(movableObject_);
			entity_->setNormaliseNormals(true);
			entity_->setCastShadows(true);
		}

		// Scale all children of this node
		node_->setScale(scale, scale, scale);

		// Move to initial position
		move(0, 0);

		//
		//addToStaticGeometry(name);
	}


	void ThingEntity
	::setEntityVisible(bool state) {
		if(state == isEntityVisible_) return;

		isEntityVisible_ = state;
		if(isEntityVisible_)
			O3dSchema::sceneManager->getSceneNode("ThingSceneNode")->addChild(node_);
		else
			O3dSchema::sceneManager->getSceneNode("ThingSceneNode")->removeChild(node_);
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
		Assert(entity_);
		entity_->setMaterialName(material);
	}


	void ThingEntity
	::move(float stepDelta, float timeSinceLastFrame) {
		Assert(thing_);
		Assert(node_);
		/*
		long d = ClientSchema::player->pos().pageDistanceSquared(thing_->pos());
		float distSq = d / static_cast<float>(COOR_DOUBLE_RES);
		if(d == 0x7fffffffL) distSq = d;

		const coor_double_t MAX_DIST_SQ = meshOut_ * meshOut_;
		const coor_double_t BILLBOARD_DIST_SQ = billboardIn_ * billboardIn_;

		// Create billboard if none exists
		if(billboardIn_ && thing_->pos().area() && !billboard_) {
			const char* bbsName = thing_->pos().area()->name();
			Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);

			Ogre::Vector3 pos
				(
				 CoorT::toFloat(thing_->pos().x_) * WORLD_SCALE + pageDeltaX_,
				 CoorT::toFloat(thing_->pos().y_) * WORLD_SCALE_Y,
				 CoorT::toFloat(thing_->pos().z_) * WORLD_SCALE + pageDeltaZ_
				 );
			billboard_ = bbs->createBillboard(pos);
			Ogre::Real scale = scale_;
			if(doScaleByRadius_) {
				// Interpolate between present radius and next radius
				scale *= CoorT::toFloat(thing_->pos().radius()) * (1.0f - stepDelta)
					+ CoorT::toFloat(thing_->pos().radius()) * stepDelta;
			}

			billboard_->setDimensions( bbs->getDefaultWidth() * scale, bbs->getDefaultHeight() * scale );
		}

		// Choose billboard texture based on distance
		if(billboard_) {
			if(distSq > BILLBOARD_DIST_SQ && thing_->pos().area()) {
				billboard_->setTexCoords(1);
			}
			else if(distSq < BILLBOARD_DIST_SQ && billboard_) {
				// Texture 0 is transparent
				billboard_->setTexCoords(0);
			}
		}

		// Check if mesh entity is visible
		bool isVisible = (meshOut_ == 0 || distSq < MAX_DIST_SQ);
		setEntityVisible(isVisible);
		if(!isVisible) {
			return;
		}
		*/
		setEntityVisible(true);

		// Are interpolations meaningful at all?
		if(!thing_->pos().isKeyFramePath(thing_->nextPos())) {
			// If not skip it
			return;
		}



		const scale_t alpha = ScaleT::fromFloat(stepDelta);

		static ViewPoint w;
		thing_->worldViewPoint(alpha, w);
		//LogMsg(thing_->name() << ": " << w.coor_.x_ << ", " << w.coor_.y_ << ", " << w.coor_.z_);

		Ogre::Vector3 pos(
				CoorT::toFloat(w.coor_.x_),
				CoorT::toFloat(w.coor_.y_),
				CoorT::toFloat(w.coor_.z_)
				);

		// Translate from Euler3 if necessary
		Quat4 face(w.face_);

		Ogre::Quaternion rot(
				QuatT::toFloat(face.w_),
				QuatT::toFloat(face.x_),
				QuatT::toFloat(face.y_),
				QuatT::toFloat(face.z_)
				);
							 
		// Set the new position
		node_->setPosition(pos);

		// Set new orientation
		node_->setOrientation(rot);

		// Check if movement mode (and thus animation) has changed (ie walk to stand, etc)
		if(prevAnim_ != anim()) {
			prevAnim_ = anim();

			// Get the animation index
			short index = O3dSchema::meshOfThing.index(thing_->name());
			// If this thing has animations
			if(index >= 0) {
				// Find animation name for the movement mode
				short animId = anim();
				const char* animation = O3dSchema::meshOfThing.animation(index, animId);
				// If this movement mode has an animation,
				// set it
				if(animation) {
					float s = O3dSchema::meshOfThing.animationSpeed(index, animId);
					setAnimation(animation, s);
				}
				else if(state_) {
					state_->setEnabled(false);
					state_ = 0;
				}

				// Check if this movment mode has a special material
				const char* material = O3dSchema::meshOfThing.material(index, anim());
				// If not, use default material
				if(!material) {
					const char* material = O3dSchema::meshOfThing.defaultMaterial(index);
				}

				// If material is defined (default or special), set it
				if(material) {
					setMaterial(material);
				}
			}
		}

		// If radius scales the model
		if(doScaleByRadius_) {
			// Interpolate between present radius and next radius
			Ogre::Real scale = CoorT::toFloat(thing_->pos().radius()) * (1.0f - stepDelta)
				+ CoorT::toFloat(thing_->pos().radius()) * stepDelta;
			scale *= scale_;

			// If scale has changed
			if(scale != currentScale_) {
				// Scale all children of this node
				node_->setScale(scale, scale, scale);

				// Store scale for future scale change checks
				currentScale_ = scale;
			}

			if(billboard_) {
				const char* bbsName = thing_->pos().area()->name();
				Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);
				billboard_->setDimensions( bbs->getDefaultWidth() * scale
										   , bbs->getDefaultHeight() * scale);
			}
		}
		// Add passed time to the animation state
		if(state_) state_->addTime(timeSinceLastFrame * speed_);
	}


}
