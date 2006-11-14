#include "SimComponentManager.hpp"
#include "SimComposite.hpp"

namespace se_core {
	SimComponentManager
	::SimComponentManager(enum SimComponentType type)
		: SimNodeComponent(type, &activeRoot()) {
	}


	SimComposite& SimComponentManager
	::activeRoot() {
		static SimComposite s("activeRoot");
		Assert(!s.hasParent());
		return s;
	}


	SimComposite& SimComponentManager
	::inactiveRoot() {
		static SimComposite s("inactiveRoot");
		return s;
	}
}
