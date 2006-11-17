#include "SimComponent.hpp"
#include "SimComposite.hpp"

namespace se_core {
	SimComponent
	::SimComponent(enum SimComponentType type, SimComposite* owner, const SimComponentFactory* factory)
			: type_(type), owner_(owner), factory_(factory) {
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


	void SimComponent
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
	}


	void SimComponent
	::init() {
	}


	void SimComponent
	::cleanup() {
	}

}
