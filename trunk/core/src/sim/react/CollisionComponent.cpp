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


#include "CollisionComponent.hpp"
#include "CollisionAreaComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "util/error/Log.hpp"
#include "../thing/Actor.hpp"
#include "../react/all.hpp"


namespace se_core {
	CollisionComponent
	::CollisionComponent(Actor* owner, PosComponent* posComponent)
		: AreaChildComponent(sct_COLLISION, owner)
		, posComponent_(posComponent) {
	}


	CollisionComponent
	::CollisionComponent(Actor* owner)
		: AreaChildComponent(sct_COLLISION, owner) {
		posComponent_ = static_cast<PosComponent*>(owner_->component(sct_POS));
		Assert(posComponent_);
	}

	CollisionComponent
	::~CollisionComponent() {
	}


	void CollisionComponent
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
		if(oldArea) {
			CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(oldArea->component(type()));
			cac->removeCollideable(*this);
		}
		if(newArea) {
			CollisionAreaComponent* cac = static_cast<CollisionAreaComponent*>(newArea->component(type()));
			cac->addCollideable(*this);
		}
	}

}
