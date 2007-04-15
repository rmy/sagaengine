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
#include "MultiO3dThingComponent.hpp"
#include "util/math/ScaleT.hpp"
#include "util/task/TaskList.hpp"
#include <OgreSceneManager.h>

using namespace se_core;
using namespace se_client;

namespace se_ogre {

	O3dThingComponent
	::O3dThingComponent(Composite* owner, const se_core::ComponentFactory* factory)
		: O3dNodeComponent(sct_RENDER, owner, factory), Task(2, 8), parentNode_(0), isVisible_(false)
		, isInitialized_(false)
		, size_(0) {
	}


	O3dThingComponent
	::~O3dThingComponent() {
		Assert(!isInitialized_);
	}


	void O3dThingComponent
	::init() {
		if(isInitialized_)
			return;
		isInitialized_ = true;


		if(!parentNode_) {
			PosComponent* a = PosComponent::Ptr(*this)->nextPos().area();
			Assert(a);
			O3dNodeComponent* c = O3dNodeComponent::get(*a);
			Assert(c);
			Assert(c->node());
			setParentNode(c->node());
		}

		O3dSchema::thingMOManager.create(this);
	}

	void O3dThingComponent
	::cleanup() {
		O3dSchema::taskList.remove(*this);
		clear();
	}


	void O3dThingComponent
	::clear() {
		if(!isInitialized_)
			return;
		isInitialized_ = false;

		while(!moList_.isEmpty()) {
			ThingMO* te = &moList_.pop();
			O3dSchema::thingMOManager.release(te);
		}
		size_ = 0;
	}

	void O3dThingComponent
	::perform() {
		if(!isActive() || isDead())
			return;
		init();
		setVisible(true);
	}


	void O3dThingComponent
	::setActive(bool state) {
		if(state) {
			if(ClientSchema::player->nextPos().area() == PosComponent::get(*this)->nextPos().area()) {
				this->setPriority(1);
				this->setWeight(0);
			}
			else {
				this->setPriority(5);
				this->setWeight(1);
			}
			O3dSchema::taskList.add(*this);
		}
		else {
			O3dSchema::taskList.remove(*this);
			setVisible(false);
			clear();
		}
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
		if(owner() == 0)
			return;

		if(!isInitialized_)
			return;

		PosComponent::Ptr tPos(*this);
		if(isDead() || !isActive() || !tPos->pos().isKeyFramePath(tPos->nextPos())) {
			// If not skip it
			setVisible(false);
			return;
		}

		setVisible(true);
		const scale_t alpha = ScaleT::fromFloat(stepDelta);
		tPos->worldViewPoint(alpha, last_);

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
		//ThingMOList::iterator_type it = firstThingMO_;
		//while(it != CompSchema::VoidList::end()) {
		ThingMOList::Iterator it(moList_);
		while(it.hasNext()) {
			ThingMO* te = &it.next(); //O3dSchema::thingMOList.next(it);
			te->move(when, stepDelta, timeSinceLastFrame);
			te->resetPos();
		}
	}


	void O3dThingComponent
	::add(ThingMO& tmo) {
		LogDetail(tmo.name());
		int s = moList_.size();
		moList_.add(tmo);
		++size_;
		Assert(moList_.size() == size_);
		Assert(size_ == s + 1);
		tmo.setParentNode(node_);
	}


	void O3dThingComponent
	::remove(ThingMO& tmo) {
		moList_.remove(tmo);
		--size_;
	}


	void O3dThingComponent
	::setParentNode(Ogre::SceneNode* sn) {
		if(sn == parentNode_)
			return;

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


	void O3dThingComponent
	::resetParentNode() {
		if(0 == parentNode_)
			return;

		if(isVisible_)
			parentNode_->removeChild(node_);
		node_->setPosition(node_->getPosition() + parentNode_->getPosition());

		parentNode_ = 0;
	}


	void O3dThingComponent
	::zoneChanged(int zoneType, Composite* newArea, Composite* oldArea) {
		if(zoneType != st_AREA)
			return;
		if(newArea) {
			O3dNodeComponent* c = static_cast<O3dNodeComponent*>(newArea->component(type()));
			Assert(c);
			setParent(*c);
			setParentNode(c->node());
		}
		else {
			setVisible(false);
			resetParent();
			resetParentNode();
		}
	}

}
