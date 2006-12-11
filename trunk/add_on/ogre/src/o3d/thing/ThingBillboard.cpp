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


#include "O3dPre.hpp"
#include "ThingMOInfo.hpp"
#include "ThingBillboard.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstdio>
#include <cstring>

using namespace se_core;

namespace se_ogre {

	ThingBillboard
	::ThingBillboard(se_core::PosComponent& thing, const ThingMOInfo& info, const ThingMOFactory& factory)
			: ThingMO(thing, info, factory) {

		const char* bbsName = info_.defaultMaterial_.get();

		Ogre::SceneManager* sm = O3dSchema::sceneManager;
		if(sm->hasBillboardSet(bbsName)) {
			billboardSet_ = sm->getBillboardSet(bbsName);
		}
		else {
			billboardSet_ = sm->createBillboardSet(bbsName);
			billboardSet_->setMaterialName(info_.defaultMaterial_.get());
			billboardSet_->setUseAccurateFacing(false);
			billboardSet_->setPointRenderingEnabled(false);
			sm->getRootSceneNode()->attachObject(billboardSet_);			
		}

		last_ = thing.nextPos().world_;
		billboard_ = billboardSet_->createBillboard(
								CoorT::toFloat(last_.coor_.x_),
								CoorT::toFloat(last_.coor_.y_),
								CoorT::toFloat(last_.coor_.z_)
								);
		Ogre::Real scale = info_.scale_;
		if(info_.doScaleByRadius_)
			scale *= CoorT::toFloat(thing_.nextPos().radius());
		billboard_->setDimensions( scale, scale );

		currentBillboardScale_ = scale;

		//billboard_->setRotation( Ogre::Radian(BrayT::toRad(last_.face_.roll_)) );
		//billboardSet_->_updateBounds();

		hasAnimation_ = true;
	}


	ThingBillboard
	::~ThingBillboard() {
		billboardSet_->removeBillboard(billboard_);
		billboard_ = 0;
	}


	void ThingBillboard
	::animate(long when, float stepDelta, float timeSinceLastFrame) {
		// Visible
		/*
		if(isVisible()) {
			billboard_->setTexcoordIndex(1);
		}
		else {
			// Texture 0 is transparent
			billboard_->setTexcoordIndex(0);
		}
		*/

		// If scale has changed
		if(currentScale_ != currentBillboardScale_) {
			// Interpolate between present radius and next radius
			const float scale = currentScale_;

			billboard_->setDimensions( scale, scale );

			// Store scale for future scale change checks
			currentBillboardScale_ = scale;

		}

		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		thing_.worldViewPoint(alpha, last_);
		billboard_->setPosition(
								CoorT::toFloat(last_.coor_.x_),
								CoorT::toFloat(last_.coor_.y_),
								CoorT::toFloat(last_.coor_.z_)
								);
		//billboard_->setPosition(node_->_getDerivedPosition());

		//billboard_->setRotation( Ogre::Radian(BrayT::toRad(last_.face_.roll_)) );
	}


}
