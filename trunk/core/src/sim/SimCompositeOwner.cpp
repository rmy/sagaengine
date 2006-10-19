#include "SimCompositeOwner.hpp"
#include "sim.hpp"

namespace se_core {
   	SimCompositeOwner
	::SimCompositeOwner(enum SimObjectType type, const char* name)
			: SimObject(type, name) {
	}


   	SimCompositeOwner
	::SimCompositeOwner(const char* name)
			: SimObject(got_SIM_COMPOSITE_OWNER, name) {
	}

}
