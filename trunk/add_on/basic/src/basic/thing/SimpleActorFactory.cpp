#include "SimpleActorFactory.hpp"

using namespace se_core;

namespace se_basic {

	SimpleActorFactory
	::SimpleActorFactory(String* name)
		: SimCompositeFactory(got_ACTOR, name)
		, isPickable_(false)
		, isCollideable_(false)
		, script_(0)
		, physics_(0)
		, collide_(0)
		, defaultAction_(0)
		, spawnPointCount_(0)
		, spawnPoints_(0) {
	}


	SimpleActorFactory
	::~SimpleActorFactory() {
		for(int i = 0; i < spawnPointCount_; ++i) {
			delete spawnPoints_[ i ];
		}
		delete defaultAction_;
		delete[] spawnPoints_;
	}


	SimComposite* SimpleActorFactory
	::create() const {
		Actor* a = new Actor(name_->get());

		a->setPickable(isPickable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setBounds(bounds_);
		a->setSpawnPoints(spawnPointCount_, spawnPoints_);
		if(defaultAction_ && defaultAction_->hasAction()) {
			LogMsg(defaultAction_->action()->name());
			a->setDefaultAction(*defaultAction_->action(), &defaultAction_->parameter());
		}
		a->setTag(tag_);

		if(script_) {
			a->setDefaultScript(script_);
		}

		if(isCollideable_) {
			CollisionComponent* cc = new CollisionComponent(a);
			cc->setCollideable(true);
		}

		createComponents(a);

		return a;
	}


	void SimpleActorFactory
	::setBounds(const BoundingBox& b) {
		bounds_ = b;
	}


	void SimpleActorFactory
	::setPickable(bool isPickable) {
		isPickable_ = isPickable;
	}


	void SimpleActorFactory
	::setCollideable(bool isCollideable) {
		isCollideable_ = isCollideable;
	}


	void SimpleActorFactory
	::setScript(const char* name) {
		script_ = SimSchema::sortedSimObjectList().script(name);
	}


	void SimpleActorFactory
	::setPhysics(const char* name) {
		physics_ = SimSchema::sortedSimObjectList().physics(name);
	}


	void SimpleActorFactory
	::setDefaultAction(const char* name) {
		defaultAction_ = new ActionAndParameter();
		defaultAction_->setAction(*SimSchema::sortedSimObjectList().action(name));
	}


	void SimpleActorFactory
	::setCollide(const char* name) {
		collide_ = SimSchema::sortedSimObjectList().collide(name);
	}



	void SimpleActorFactory
	::setSpawnPoints(int count, ViewPoint* const* spawnPoints) {
		spawnPoints_ = new ViewPoint*[ count ];

		spawnPointCount_ = count;
		for(int i = 0; i < spawnPointCount_; ++i) {
			spawnPoints_[ i ] = spawnPoints[ i ];
		}
	}

}
