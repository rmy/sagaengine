#include "Component.hpp"
#include "Composite.hpp"

namespace se_core {
	Component
	::Component(int type, Composite* owner, const ComponentFactory* factory)
			: type_(type), owner_(owner), factory_(factory) {
		owner_->addComponent(*this);
	}


	Component
	::~Component() {
		owner_->removeComponent(*this);
		owner_ = 0;
	}


	bool Component
	::isActive() {
		return owner_->isActive();
	}

	bool Component
	::isDead() {
		return owner_->isDead();
	}


	void Component
	::areaChanged(Composite* newArea, Composite* oldArea) {
	}


	void Component
	::init() {
	}


	void Component
	::cleanup() {
	}

}
