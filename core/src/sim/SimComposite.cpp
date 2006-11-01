#include "SimComposite.hpp"
#include "stat/SimComponentIterator.hpp"
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


	void SimComposite
	::setActive(bool state) {
		if(state == isActive_) return;

		isActive_ = state;
		SimComponentIterator it(components_);
		while(it.hasNext()) {
			it.next().setActive(state);
		}
	}


	SimComponent*  SimComposite
	::component(int type) {
		SimComponentIterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			if(c.type() == type) {
				return &c;
			}
		}
		//return composites_.lookup(type);
		return 0;
	}


	void SimComposite
	::addComponent(SimComponent& c) {
		components_.add(c);
	}

	void SimComposite
	::removeComponent(SimComponent& c) {
		components_.remove(c);
	}

}
