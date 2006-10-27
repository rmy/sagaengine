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


#include "ThingLight.hpp"
#include "ThingMOInfo.hpp"
#include "all.hpp"
#include "O3dPre.hpp"
#include "../config/o3d_config.hpp"
#include "../schema/O3dSchema.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "o3d/thing/all.hpp"


using namespace se_core;


namespace se_ogre {
	ThingLight
	::ThingLight(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory), light_(0) {

		// Create a unique entity name
		char name[128];
		static int pool = 1000;
		//sprintf(name, "%d-%s", thing_.id(), thing_.name());
		sprintf(name, "%d-%s", ++pool, thing_.name());

		// Create entity
		light_ =  O3dSchema::sceneManager->createLight(name);
		light_->setType(Ogre::Light::LT_POINT);
		light_->setCastShadows(info.isShadowCaster_);
		light_->setDiffuseColour(Ogre::ColourValue(.7, .7, .7));
		node_->attachObject(light_);

		float range = 128;
		float constant = 1;
		float linear = 0.001;
		float quadratic =  0.001;
		light_->setAttenuation(range, constant, linear, quadratic);

		hasAnimation_ = false;
	}


	ThingLight
	::~ThingLight() {
		O3dSchema::sceneManager->destroyMovableObject(light_);
		light_ = 0;
	}



}
