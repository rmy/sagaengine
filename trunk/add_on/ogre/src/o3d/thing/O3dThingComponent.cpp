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


#include "O3dThingComponent.hpp"
#include "../schema/O3dSchema.hpp"
#include "ThingMO.hpp"

using namespace se_core;
using namespace se_client;

namespace se_ogre {

	O3dThingComponent
	::O3dThingComponent(Actor* owner)
		: SimComponent(sct_RENDER, owner), parentNode_(0), isVisible_(false)
		, firstThingMO_(ThingMOList::end()) {

		// Create things node, and add it to scene manager
		node_ = O3dSchema::sceneManager->createSceneNode();
		LogMsg(owner->name());
	}


	O3dThingComponent
	::~O3dThingComponent() {
		LogMsg(owner_->name());
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::end()) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			O3dSchema::thingMOManager.release(te);
		}

		setVisible(false);

		node_->removeAndDestroyAllChildren();
		O3dSchema::sceneManager->destroySceneNode(node_->getName());
		node_ = 0;
	}


	void O3dThingComponent
	::setActive(bool state) {
	}


	void O3dThingComponent
	::setVisible(bool state) {
		if(isVisible_ == state)
			return;

		isVisible_ = state;
		if(isVisible_)
			parentNode_->addChild(node_);
		else
			parentNode_->removeChild(node_);
	}


	void O3dThingComponent
	::move(long when, float stepDelta, float timeSinceLastFrame) {
		// Are interpolations meaningful at all?
		if(owner_ == 0)
			return;
		if(isDead() || !isActive() || !owner_->pos().isKeyFramePath(owner_->nextPos())) {
			// If not skip it
			setVisible(false);
			return;
		}

		setVisible(true);
		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		owner_->worldViewPoint(alpha, last_);

		// Translate from Euler3 if necessary
		Quat4 face(last_.face_);

		Ogre::Vector3 pos(
				CoorT::toFloat(last_.coor_.x_),
				CoorT::toFloat(last_.coor_.y_),
				CoorT::toFloat(last_.coor_.z_)
				);

		Ogre::Quaternion rot(
				QuatT::toFloat(face.w_),
				QuatT::toFloat(face.x_),
				QuatT::toFloat(face.y_),
				QuatT::toFloat(face.z_)
				);


		// Set the new position
		node_->setPosition(pos - parentNode_->getPosition());

		// Set new orientation
		node_->setOrientation(rot);

		// Move children
		ThingMOList::iterator_type it = firstThingMO_;
		while(it != ThingMOList::end()) {
			ThingMO* te = O3dSchema::thingMOList.next(it);
			te->move(when, stepDelta, timeSinceLastFrame);
			te->resetPos();
		}
	}


	void O3dThingComponent
	::add(ThingMO& tmo) {
		O3dSchema::thingMOList.add(&tmo, firstThingMO_);
		tmo.setParentNode(node_);
	}


	void O3dThingComponent
	::remove(ThingMO& tmo) {
		O3dSchema::thingMOList.remove(&tmo, firstThingMO_);
	}


	void O3dThingComponent
	::setParentNode(Ogre::SceneNode* sn) {
		if(sn == parentNode_)
			return;
		LogMsg(owner_->name());

		if(parentNode_) {
			if(isVisible_)
				parentNode_->removeChild(node_);
			node_->setPosition(node_->getPosition() + parentNode_->getPosition());
		}
		parentNode_ = sn;
		node_->setPosition(node_->getPosition() - parentNode_->getPosition());

		if(isVisible_)
			parentNode_->addChild(node_);
	}

}