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


#include "ThingStaticGeometry.hpp"
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
	ThingStaticGeometry
	::ThingStaticGeometry(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory), entity_(0) {

		// Create a unique entity name
		char name[128];
		static int pool = 1000;
		sprintf(name, "sg-%d-%s", ++pool, thing_.name());

		// Create entity
		Ogre::MovableObject* mo = O3dSchema::sceneManager->createMovableObject(name, Ogre::EntityFactory::FACTORY_TYPE_NAME, &info_.params_);
		//node_->attachObject(mo);
		entity_ = static_cast<Ogre::Entity*>(mo);
		hasAnimation_ = false;

		try {
			addToStaticGeometry(name);
		}
		catch(...) {}
	}


	ThingStaticGeometry
	::~ThingStaticGeometry() {
		O3dSchema::sceneManager->destroyMovableObject(entity_);
		entity_ = 0;
	}


	void ThingStaticGeometry
	::addToStaticGeometry(const char* name) {
		const char* staticName = thing_.nextPos().area()->name();
		Ogre::SceneManager* sm = O3dSchema::sceneManager;
		staticGeometry_ = sm->getStaticGeometry(staticName);

		se_core::ViewPoint& nextPos = thing_.nextPos().world_;
		Quat4 face(nextPos.face_);

		///////
		Ogre::Vector3 pos(
			CoorT::toFloat(nextPos.coor_.x_),
			CoorT::toFloat(nextPos.coor_.y_),
			CoorT::toFloat(nextPos.coor_.z_)
			);

		Ogre::Quaternion rot(
			QuatT::toFloat(face.w_),
			QuatT::toFloat(face.x_),
			QuatT::toFloat(face.y_),
			QuatT::toFloat(face.z_)
			);


		Ogre::Real scale = info_.scale_;
		// If radius scales the model
		if(info_.doScaleByRadius_) {
			// Interpolate between present radius and next radius
			scale *= CoorT::toFloat(thing_.nextPos().radius());

			// If scale has changed
			if(scale != currentScale_) {
				// Scale all children of this node
				//node_->setScale(scale, scale, scale);

				// Store scale for future scale change checks
				currentScale_ = scale;
			}
		}

		Ogre::Vector3 s(scale, scale, scale);
		staticGeometry_->addEntity(entity_, pos, rot, s);
	}

}
