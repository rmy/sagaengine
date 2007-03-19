#include "SimComponentManager.hpp"
#include "SimComposite.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_core {
	SimComponentManager
	::SimComponentManager(enum SimComponentType type)
		: SimNodeComponent(type, &activeRoot()) {
	}


	Composite& SimComponentManager
	::activeRoot() {
		return CompSchema::activeRoot();
	}


	Composite& SimComponentManager
	::inactiveRoot() {
		return CompSchema::inactiveRoot();
	}
}
