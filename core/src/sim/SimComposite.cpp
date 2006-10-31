#include "SimComposite.hpp"
#include "sim.hpp"

namespace se_core {
   	SimComposite
	::SimComposite(enum SimObjectType type, const char* name)
		: SimObject(type, name), isActive_(false), isDead_(false) {
	}


   	SimComposite
	::SimComposite(const char* name)
			: SimObject(got_SIM_COMPOSITE_OWNER, name) {
	}

}
