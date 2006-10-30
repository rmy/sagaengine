#include "SimComponent.hpp"
#include "thing/Actor.hpp"

namespace se_core {
	bool SimComponent
	::isActive() {
		return owner_->isActive();
	}

	bool SimComponent
	::isDead() {
		return owner_->isDead();
	}

}
