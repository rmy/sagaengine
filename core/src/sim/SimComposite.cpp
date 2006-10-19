#include "SimComposite.hpp"
#include "thing/Actor.hpp"

namespace se_core {
	bool SimComposite
	::isActive() {
		return owner_->isActive();
	}

	bool SimComposite
	::isDead() {
		return owner_->isDead();
	}

}
