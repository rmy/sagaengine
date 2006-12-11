#include "SimComposite.hpp"
#include "SimComponent.hpp"
#include "SimComponentFactory.hpp"
#include "SimComponentManager.hpp"
#include "stat/MultiSimComponent.hpp"
#include "stat/MultiSimComposite.hpp"
#include "sim.hpp"

namespace se_core {
   	SimComposite
	::SimComposite(enum SimObjectType type, const char* name)
		: SimObject(type, name)
		, ptr_(this), tag_(0), parent_(0), isActive_(false), isDead_(false) {
	}


   	SimComposite
	::SimComposite(const char* name)
		: SimObject(got_SIM_COMPOSITE, name)
		, ptr_(this), tag_(0), parent_(0), isActive_(false), isDead_(false) {
	}


	SimComposite
	::~SimComposite() {
	}


	void SimComposite
	::setActive(bool state, bool doTraverseChildren) {
		if(state == isActive_) return;

		isActive_ = state;

		if(!state && doTraverseChildren) {
			MultiSimComposite::Iterator composites(children_);
			while(composites.hasNext()) {
				SimComposite& c = composites.next();
				c.setActive(state);
			}
		}

		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			it.next().setActive(state);
		}

		if(state && doTraverseChildren) {
			MultiSimComposite::Iterator composites(children_);
			while(composites.hasNext()) {
				SimComposite& c = composites.next();
				c.setActive(state);
			}
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


	const SimComponent*  SimComposite
	::component(int type) const {
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			const SimComponent& c = it.next();
			if(c.type() == type) {
				return &c;
			}
		}
		//return composites_.lookup(type);
		return 0;
	}


	void SimComposite
	::releaseComponents() {
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			if(c.factory()) {
				c.factory()->release(&c);
			}
		}
	}


	void SimComposite
	::addComponent(SimComponent& c) {
		//LogMsg("Added component of type " << c.type() << " to " << c.owner()->name());
		AssertFatal(component(c.type()) == 0, name() << ": " << c.type());
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

		setActive(false, true);
	}


	void SimComposite
	::setParent(SimComposite& p) {
		if(parent_ == &p)
			return;

		if(parent_)
			parent_->removeChild(*this);

		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.parentChanged(&p, parent_);
		}

		parent_ = &p;
		parent_->addChild(*this);

		setActive(parent_->isActive(), true);
	}


	void SimComposite
	::areaChanged(SimComposite* newArea, SimComposite* oldArea) {
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.areaChanged(newArea, oldArea);
		}
	}


	void SimComposite
	::init(bool doTraverseChildren) {
		// Init self first...
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.init();
		}

		// ... then children
		if(doTraverseChildren) {
			MultiSimComposite::Iterator composites(children_);
			while(composites.hasNext()) {
				SimComposite& c = composites.next();
				c.init();
			}
		}
	}


	void SimComposite
	::cleanup(bool doTraverseChildren) {
		setActive(false, true);
		// Cleanup children first...
		if(doTraverseChildren) {
			MultiSimComposite::Iterator composites(children_);
			while(composites.hasNext()) {
				SimComposite& c = composites.next();
				c.cleanup();
			}
		}

		// ... then self
		resetParent();
		MultiSimComponent::Iterator it(components_);
		while(it.hasNext()) {
			SimComponent& c = it.next();
			c.cleanup();
		}
	}


}
