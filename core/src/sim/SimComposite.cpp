#include "SimComposite.hpp"
#include "SimComponent.hpp"
#include "SimComponentFactory.hpp"
#include "SimComponentManager.hpp"
#include "stat/MultiSimComponent.hpp"
#include "sim.hpp"


namespace se_core {

	SimComposite
	::SimComposite(const CompositeFactory* f)
			: Composite(f) {
	}

}
