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


#ifndef o3d_thing_ThingEntity_hpp
#define o3d_thing_ThingEntity_hpp

#include "O3dPre.H"
#include "o3d_thing.hpp"
#include "sim/thing/sim_thing.hpp"

namespace se_ogre {

	/**
	 * An entity visually representing a se_core::Thing
	 * in 3D.
	 */
	class ThingEntity {
	public:
		/**
		 * @param thing The thing that this entity is visually representing.
		 */
		ThingEntity(se_core::Thing& thing);
		~ThingEntity();

		/**
		 * Move the the entity, update animations and material.
		 * @param Time sinse last move
		 */
		void move(float stepDelta, float timeSinceLastFrame);

		/**
		 * Check if this entity is controlled by a specific se_core::Thing
		 * object
		 */
		bool hasThing(se_core::Thing& thing) { return (thing_ == &thing); }
		void addToStaticGeometry(const char* name);

		short anim();


	protected:
		bool endsWith(const char* s, const char* postfix);

		/**
		 * Set the mesh entity.
		 */
		//void setEntity(const char* name, const char* mesh, float scale);
		void setEntity(const char* name, const char* factory, Ogre::NameValuePairList* params, float scale);

		void setEntityVisible(bool state);

		/**
		 * Set the active animation.
		 */
		void setAnimation(const char* anim, float speed);

		/**
		 * Set the active material.
		 */
		void setMaterial(const char* mat);

		Ogre::SceneNode* node_;
		Ogre::Entity* entity_;
		Ogre::Billboard* billboard_;
		Ogre::MovableObject* movableObject_;
		Ogre::AnimationState* state_;
		Ogre::Real speed_;
		bool doScaleByRadius_;
		Ogre::Real currentScale_;
		Ogre::Real scale_;
		Ogre::Real meshOut_;
		Ogre::Real billboardIn_;
		se_core::Thing* thing_;
		//EntityMaterial* tempMaterial_;
		short prevAnim_;
		bool isEntityVisible_;
	};

}


#endif
