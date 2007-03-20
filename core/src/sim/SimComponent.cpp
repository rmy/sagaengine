#include "SimComponent.hpp"
#include "comp/Composite.hpp"
#include "SimComponentFactory.hpp"
#include "thing/Actor.hpp"

namespace se_core {
	SimComponent
	::SimComponent(int type, Composite* owner, const SimComponentFactory* factory)
			: Component(type, owner, factory) {
	}

	Actor* SimComponent
	::toActor() {
		return static_cast<Actor*>(owner_->component(sct_BLOB));
	}

	const Actor* SimComponent
	::toActor() const {
		return static_cast<Actor*>(owner_->component(sct_BLOB));
	}

}
