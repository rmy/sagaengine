#include "SimplePlayerFactory.hpp"


using namespace se_core;

namespace logic {

	SimplePlayerFactory
	::SimplePlayerFactory(String* name)
		: SimpleActorFactory(name) {
	}


	SimplePlayerFactory
	::~SimplePlayerFactory() {
	}


	Thing* SimplePlayerFactory
	::create() const {
		Player* a = new Player(name_->get());

		a->setPickable(isPickable_);
		a->setCollideable(isCollideable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setRadius(radius_);
		a->setSpawnPoints(spawnPointCount_, spawnPoints_);

		if(script_) {
			a->setDefaultScript(script_);
		}

		return a;
	}

}
