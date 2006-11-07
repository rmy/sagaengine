#include "SimpleActorFactory.hpp"

using namespace se_core;

namespace se_basic {

	SimpleActorFactory
	::SimpleActorFactory(String* name)
		: ThingFactory(got_ACTOR, name)
		, radius_(COOR_RES)
		, isPickable_(false)
		, isCollideable_(false)
		, script_(0)
		, physics_(0)
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


	Thing* SimpleActorFactory
	::create() const {
		Actor* a = new Actor(name_->get());

		a->setPickable(isPickable_);
		a->setCollideable(isCollideable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setRadius(radius_);
		a->setSpawnPoints(spawnPointCount_, spawnPoints_);
		if(defaultAction_ && defaultAction_->hasAction()) {
			LogMsg(defaultAction_->action()->name());
			a->setDefaultAction(*defaultAction_->action(), &defaultAction_->parameter());
		}

		if(script_) {
			a->setDefaultScript(script_);
		}

		return a;
	}


	void SimpleActorFactory
	::setRadius(float r) {
		radius_ = static_cast<coor_t>(r * COOR_RES);
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
