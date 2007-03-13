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


#include "ThingDebug.hpp"
#include "ThingMOInfo.hpp"
#include "all.hpp"
#include "O3dPre.hpp"
#include "../config/o3d_config.hpp"
#include "../schema/O3dSchema.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "o3d/area/O3dManager.hpp"
#include "o3d/thing/all.hpp"
#include "EntityUtil.hpp"


using namespace se_core;


namespace se_ogre {
	ThingDebug
	::ThingDebug(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory), entity_(0) {

		// Create a unique entity name
		char name[128];
		static int pool = 1000;
		sprintf(name, "%d-debug-%s", ++pool, thing_.name());

		// Create entity
		entity_ = O3dSchema::sceneManager->createEntity(name, "cylinder.mesh");
		node_->attachObject(entity_);
		entity_->setNormaliseNormals(true);
		entity_->setCastShadows(false);

		// Get default material, if one
		//const char* material = info_.defaultMaterial_.get();
		// If material is defined (default or special), set it
		//if(material) {
		//	setMaterial(material);
		//}

		hasAnimation_ = true;
	}


	ThingDebug
	::~ThingDebug() {
		if(O3dSchema::sceneManager)
			O3dSchema::sceneManager->destroyMovableObject(entity_);
		entity_ = 0;
	}


	void ThingDebug
	::setMaterial(const char* material) {
		entity_->setMaterialName(material);
	}


	void ThingDebug
	::animate(long when, float stepDelta, float timeSinceLastFrame) {
		bool shouldShow = thing_.isActive() && O3dSchema::worldManager->isShowingDebugInfo();
		if(shouldShow != isVisible()) {
			setVisible(shouldShow);
		}
		if(!isVisible())
			return;

		// Interpolate between present radius and next radius
		BoundingBox& b = thing_.nextPos().bounds_;
		Point3 center;
		b.center(center);
		//center.add(thing_.nextPos().worldCoor());

		///////
		Ogre::Vector3 nextPos
			(
				CoorT::toFloat(center.x_),
				CoorT::toFloat(b.minY_),
				CoorT::toFloat(center.z_)
			 );

		float scale = CoorT::toFloat(thing_.nextPos().radius());

		Ogre::Vector3 s;
		s.x = CoorT::half(b.maxX_ - b.minX_);
		s.y = b.maxY_ - b.minY_;
		s.z = CoorT::half(b.maxZ_ - b.minZ_);


		// Scale all children of this node
		node_->setPosition(nextPos);
		node_->setScale(s);
	}


}
