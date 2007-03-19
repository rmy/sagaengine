#include "SimComponent.hpp"
#include "SimComposite.hpp"
#include "SimComponentFactory.hpp"

namespace se_core {
	SimComponent
	::SimComponent(int type, Composite* owner, const SimComponentFactory* factory)
			: Component(type, owner, factory) {
	}


}
