#include "SimComponent.hpp"
#include "SimComposite.hpp"

namespace se_core {
	SimComponent
	::SimComponent(enum SimComponentType type, SimComposite* owner)
			: type_(type), owner_(owner) {
		owner_->addComponent(*this);
	}


	SimComponent
	::~SimComponent() {
		owner_->removeComponent(*this);
		owner_ = 0;
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
