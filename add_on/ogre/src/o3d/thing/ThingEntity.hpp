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

#include "O3dPre.hpp"
#include "ThingMO.hpp"
#include "o3d_thing.hpp"
#include "sim/thing/sim_thing.hpp"

namespace se_ogre {

	/**
	 * An entity visually representing a se_core::Thing
	 * in 3D.
	 */
	class _SeOgreExport ThingEntity : public ThingMO {
	public:
		/**
		 * @param thing The thing that this entity is visually representing.
		 */
		ThingEntity(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory);
		~ThingEntity();

		/**
		 * Update animations and material.
		 * @param Time sinse last move
		 */
		void animate(long when, float stepDelta, float timeSinceLastFrame);

		void addToStaticGeometry(const char* name);

		short anim(int channel);


	protected:
		/**
		 * Set the active animation.
		 */
		void setAnimation(int channel, const se_core::Anim& anim);

		/**
		 * Set the active material.
		 */
		void setMaterial(const char* mat);

		Ogre::Entity* entity_;

		enum { MAX_ANIMS = se_core::Pos::MAX_ANIMS };
		Ogre::AnimationState* state_[MAX_ANIMS];
		float speed_[MAX_ANIMS];
		short prevAnim_[MAX_ANIMS];
	};

}


#endif
