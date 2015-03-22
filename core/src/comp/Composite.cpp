#include "Composite.hpp"
#include "Component.hpp"
#include "CompositeFactory.hpp"
#include "comp/list/ComponentList.hpp"
#include "comp/list/CompositeList.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	Composite::id_type Composite
	::idPool() {
		static id_type nextId = 0;
		return ++nextId;
	}


	Composite
	::Composite(const CompositeFactory* factory)
		: id_(Composite::idPool()), name_(0), factory_(factory), tag_(0), isActive_(false), isDead_(false), debugLevel_(0), parent_(0), ptr_(this) {
		name_ = factory_->name();
		for(int i = 0; i < FAST_COMPONENT_COUNT; ++i) {
			fastComponents_[i] = 0;
		}
		components_.clear();
		children_.clear();
	}


	Composite
	::Composite(const CompositeFactory* factory, const char* name)
			: id_(Composite::idPool()), name_(name), factory_(factory), tag_(0), isActive_(false), isDead_(false), debugLevel_(0), parent_(0), ptr_(this) {
		for(int i = 0; i < FAST_COMPONENT_COUNT; ++i) {
			fastComponents_[i] = 0;
		}
		components_.clear();
		children_.clear();
	}


	Composite
	::~Composite() {
		releaseComponents();
		if(factory_ && name_ != factory_->name()) {
			delete[] name_;
		}
	}


	const char* Composite
	::name() const {
		return name_;
	}


	int Composite
	::type() const {
		return factory_->type();
	}


	void Composite
	::setActive(bool state, bool doTraverseChildren) {
		if(state == isActive_) return;

		isActive_ = state;

		// Children is inactive before *this
		if(!state && doTraverseChildren) {
			CompositeList::Iterator composites(children_);
			while(composites.hasNext()) {
				Composite& c = composites.next();
				c.setActive(state);
			}
		}

		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			it.next().setActive(state);
		}

		// Children goes active after *this
		if(state && doTraverseChildren) {
			CompositeList::Iterator composites(children_);
			while(composites.hasNext()) {
				Composite& c = composites.next();
				c.setActive(state);
			}
		}

	}


	Component*  Composite
	::component(int type) {
		if(type < FAST_COMPONENT_COUNT) {
			Component* c = fastComponents_[ type ];
			return c;
		}
		else {
			ComponentList::Iterator it(components_);
			while(it.hasNext()) {
				Component& c = it.next();
				if(c.type() == type) {
					return &c;
				}
			}
		}
		//return composites_.lookup(type);
		return 0;
	}


	const Component*  Composite
	::component(int type) const {
		if(type < FAST_COMPONENT_COUNT) {
			const Component* c = fastComponents_[ type ];
			return c;
		}
		else {
			ComponentList::Iterator it(components_);
			while(it.hasNext()) {
				const Component& c = it.next();
				if(c.type() == type) {
					return &c;
				}
			}
		}
		//return composites_.lookup(type);
		return 0;
	}


	void Composite
	::releaseComponents() {
		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.release();
		}
	}


	void Composite
	::addComponent(Component& c) {
		//LogDetail("Added component of type " << c.type() << " to " << c.owner()->name());
		AssertFatal(component(c.type()) == 0, name() << ": " << c.type());
		components_.add(c);
		if(c.type() < FAST_COMPONENT_COUNT) {
			fastComponents_[ c.type() ] = &c;
		}
	}


	void Composite
	::removeComponent(Component& c) {
		if(c.type() < FAST_COMPONENT_COUNT) {
			fastComponents_[ c.type() ] = 0;
		}
		components_.remove(c);
	}


	void Composite
	::addChild(Composite& node) {
		children_.add(node);
	}


	void Composite
	::removeChild(Composite& node) {
		children_.remove(node);
	}


	void Composite
	::resetParent() {
		if(parent_)
			parent_->removeChild(*this);

		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.parentChanged(0, parent_);
		}

		parent_ = 0;

		setActive(false, true);
	}


	void Composite
	::setParent(Composite& p) {
		if(parent_ == &p)
			return;

		if(parent_)
			parent_->removeChild(*this);

		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.parentChanged(&p, parent_);
		}

		parent_ = &p;
		parent_->addChild(*this);

		setActive(parent_->isActive(), true);
	}


	void Composite
	::zoneChanged(int type, Composite* newZone, Composite* oldZone) {
		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.zoneChanged(type, newZone, oldZone);
		}
	}


	void Composite
	::init(bool doTraverseChildren) {
		// Init self first...
		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.init();
		}

		// ... then children
		if(doTraverseChildren) {
			CompositeList::Iterator composites(children_);
			while(composites.hasNext()) {
				Composite& c = composites.next();
				c.init();
			}
		}
	}


	void Composite
	::cleanup(bool doTraverseChildren) {

		// Cleanup children first...
		if(doTraverseChildren) {
			CompositeList::Iterator composites(children_);
			while(composites.hasNext()) {
				Composite& c = composites.next();
				c.cleanup(true);
			}
		}

		// ... then self
		setActive(false, true);
		resetParent();

		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.cleanup();
		}
	}


	void Composite
	::scheduleForDestruction() {
		if(isDead_) return;

		ComponentList::Iterator it(components_);
		while(it.hasNext()) {
			Component& c = it.next();
			c.setDead();
		}

		isDead_ = true;
	}
}
