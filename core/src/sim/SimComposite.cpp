#include "SimComposite.hpp"
#include "SimComponent.hpp"
#include "stat/MultiSimComponent.hpp"
#include "sim.hpp"

namespace se_core {
   	SimComposite
	::SimComposite(enum SimObjectType type, const char* name)
		: SimObject(type, name), ptr_(this), tag_(0), parent_(0), isActive_(false), isDead_(false) {
	}


   	SimComposite
	::SimComposite(const char* name)
		: SimObject(got_SIM_COMPOSITE_OWNER, name), ptr_(this), tag_(0), parent_(0), isActive_(false), isDead_(false) {
	}


	void SimComposite
	::setActive(bool state) {
		if(state == isActive_) return;

		isActive_ = state;
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			it.next().setActive(state);
		}
	}


	SimComponent*  SimComposite
	::component(int type) {
		MultiSimComponent::Iterator it(components_);
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


	void SimComposite
	::addChild(SimComposite& node) {
		children_.add(node);
	}


	void SimComposite
	::removeChild(SimComposite& node) {
		children_.remove(node);
	}


	void SimComposite
	::resetParent() {
		if(parent_)
			parent_->removeChild(*this);

		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.parentChanged(0, parent_);
		}

		parent_ = 0;
	}


	void SimComposite
	::setParent(SimComposite& p) {
		if(parent_)
			parent_->removeChild(*this);

		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.parentChanged(&p, parent_);
		}

		parent_ = &p;
		parent_->addChild(*this);
	}

}
