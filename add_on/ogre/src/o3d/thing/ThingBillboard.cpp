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
#include "ThingMOInfo.hpp"
#include "ThingBillboard.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstdio>
#include <cstring>

using namespace se_core;

namespace se_ogre {

	ThingBillboard
	::ThingBillboard(se_core::PosNode& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
		: ThingMO(thing, info, factory) {

		const char* bbsName = thing_.pos().area()->name();
		Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);

		Ogre::Vector3 pos
			(
			 CoorT::toFloat(thing_.pos().worldCoor().x_),
			 CoorT::toFloat(thing_.pos().worldCoor().y_),
			 CoorT::toFloat(thing_.pos().worldCoor().z_)
			 );
		billboard_ = bbs->createBillboard(pos);
		Ogre::Real scale = info_.scale_;
		if(info_.doScaleByRadius_)
			scale *= CoorT::toFloat(thing_.nextPos().radius());
		billboard_->setDimensions( bbs->getDefaultWidth() * scale, bbs->getDefaultHeight() * scale );
		currentBillboardScale_ = scale;
	}


	ThingBillboard
	::~ThingBillboard() {
		const char* bbsName = thing_.pos().area()->name();
		Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);
		bbs->removeBillboard(billboard_);
		billboard_ = 0;
	}


	void ThingBillboard
	::animate(float stepDelta, float timeSinceLastFrame) {
		// Visible
		if(isVisible()) {
			billboard_->setTexcoordIndex(1);
		}
		else {
			// Texture 0 is transparent
			billboard_->setTexcoordIndex(0);
		}

		// If scale has changed
		if(currentScale_ != currentBillboardScale_) {
			// Interpolate between present radius and next radius
			const float scale = currentScale_;

			const char* bbsName = thing_.pos().area()->name();
			Ogre::BillboardSet* bbs = O3dSchema::sceneManager->getBillboardSet(bbsName);
			billboard_->setDimensions( bbs->getDefaultWidth() * scale
										   , bbs->getDefaultHeight() * scale);

			// Store scale for future scale change checks
			currentBillboardScale_ = scale;
		}

	}


}
