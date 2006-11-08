#include "SimpleCameraFactory.hpp"


using namespace se_core;

namespace se_basic {

	SimpleCameraFactory
	::SimpleCameraFactory(String* name)
		: SimpleActorFactory(name) {
	}


	SimpleCameraFactory
	::~SimpleCameraFactory() {
	}


	SimComposite* SimpleCameraFactory
	::create() const {
		Camera* a = new Camera(name_->get());

		a->setPickable(isPickable_);
		a->setCollideable(isCollideable_);
		a->setDefaultPhysics(physics_);
		a->setCollide(collide_);
		a->nextPos().setRadius(radius_);
		LogMsg(spawnPointCount_ << " " << name());
		a->setSpawnPoints(spawnPointCount_, spawnPoints_);

		if(script_) {
			a->setDefaultScript(script_);
		}
		a->setTag(tag_);

		return a;
	}

}
