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


#include "ThingMOInfo.hpp"
#include "ThingParticle.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstdio>
#include <cstring>

namespace se_ogre {

	ThingParticle
	::ThingParticle(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory) {

		// Create a unique entity name
		char name[128];
		sprintf(name, "%d-%s", thing_.owner()->id(), thing_.name());

		// Set the mesh
		movableObject_ = O3dSchema::sceneManager->createMovableObject(name, info_.movableObjectType_.get(), &info_.params_);
		node_->attachObject(movableObject_);
		particle_ = static_cast<Ogre::ParticleSystem*>(movableObject_);
	}


	ThingParticle
	::~ThingParticle() {
		O3dSchema::sceneManager->destroyMovableObject(movableObject_);
		movableObject_ = 0;
	}


	void ThingParticle
	::animate(long when, float stepDelta, float timeSinceLastFrame) {
		float p = thing_.pos().anim(0).pos();
		float w = thing_.pos().anim(0).weight();
	}


}
