#include "SimComponent.hpp"
#include "SimComposite.hpp"
#include "thing/Actor.hpp"

namespace se_core {
	SimComponent
	::SimComponent(enum SimComponentType type, Actor* owner)
			: type_(type), owner_(owner) {
		owner_->addComponent(*this);
	}


	SimComponent
	::~SimComponent() {
		owner_->removeComponent(*this);
	}


	bool SimComponent
	::isActive() {
		return owner_->isActive();
	}

	bool SimComponent
	::isDead() {
		return owner_->isDead();
	}

}
