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
		//LogWarning(owner_->name() << "(" << type() << ")");
		owner_->removeComponent(*this);
		owner_ = 0;
	}


	const char* Component
	::name() const {
		return owner()->name();
	}

	bool Component
	::isActive() {
		return owner_->isActive();
	}

	bool Component
	::isDead() const {
		return owner_->isDead();
	}


	void Component
	::zoneChanged(int type, Composite* newArea, Composite* oldArea) {
	}


	void Component
	::init() {
	}


	void Component
	::cleanup() {
	}

}
