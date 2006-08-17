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


#include "ThingMO.hpp"
#include "ThingMOInfo.hpp"
#include "../schema/O3dSchema.hpp"
#include "util/math/all.hpp"

using namespace se_core;


namespace se_ogre {

	ThingMO
	::ThingMO(const ThingMOFactory& factory, const ThingMOInfo& info) 
		: factory_(factory), info_(info), hasAnimation_(false), isVisible_(false), currentScale_(1) {

		// Create things node, and add it to scene manager
		node_ = O3dSchema::sceneManager->createSceneNode();

		// Scale all children of this node
		Ogre::Real s = info_.scale_;
		node_->setScale(s, s, s);

	}


	ThingMO
	::~ThingMO() {
	}


	void ThingMO
	::move(float stepDelta) {
		setVisible(true);

		// Are interpolations meaningful at all?
		if(!thing_->pos().isKeyFramePath(thing_->nextPos())) {
			// If not skip it
			return;
		}

		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		ViewPoint w;
		thing_->worldViewPoint(alpha, w);

		// Translate from Euler3 if necessary
		Quat4 face(w.face_);

		Ogre::Vector3 pos(
				CoorT::toFloat(w.coor_.x_),
				CoorT::toFloat(w.coor_.y_),
				CoorT::toFloat(w.coor_.z_)
				);

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

		// If radius scales the model
		if(info_.doScaleByRadius_) {
			Ogre::Real r1 = CoorT::toFloat(thing_->pos().radius());
			Ogre::Real r2 = CoorT::toFloat(thing_->nextPos().radius());
			Ogre::Real s = r1 - stepDelta * (r2 - r1);

			// If scale has changed
			if(s != currentScale_) {
				// Store scale for future scale change checks
				currentScale_ = s;

				//
				s *= info_.scale_;

				// Scale all children of this node
				node_->setScale(s, s, s);

			}
		}
		
		if(hasAnimation_)
			animate(stepDelta, 0);
	}


	void ThingMO
	::setVisible(bool state) {
		if(state == isVisible_) return;

		isVisible_ = state;
		if(isVisible_)
			O3dSchema::sceneManager->getSceneNode("ThingSceneNode")->addChild(node_);
		else
			O3dSchema::sceneManager->getSceneNode("ThingSceneNode")->removeChild(node_);
	}

}
