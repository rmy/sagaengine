#include "SimplePlayerFactory.hpp"


using namespace se_core;

namespace se_basic {

	SimplePlayerFactory
	::SimplePlayerFactory(String* name)
		: SimpleActorFactory(name) {
	}


	SimplePlayerFactory
	::~SimplePlayerFactory() {
	}


	SimComposite* SimplePlayerFactory
	::create() const {
		Player* a = new Player(name_->get());

		a->setPickable(isPickable_);
		a->setCollideable(isCollideable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setBounds(bounds_);
		a->setSpawnPoints(spawnPointCount_, spawnPoints_);

		if(script_) {
			a->setDefaultScript(script_);
		}
		a->setTag(tag_);

		createComponents(a);

		return a;
	}

}
