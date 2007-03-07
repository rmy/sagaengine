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
		, spawnPoints_(0)
		, signalSendId_(-1)
		, signalRecieveMask_(0)
		, signal_(0) {
		anim_[0].setWeight(1);
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

		if(isCollideable_ || collide_) {
			CollisionComponent* cc = new CollisionComponent(a);
			cc->setCollide(collide_);
			cc->setCollideable(isCollideable_);
		}

		if(signalSendId_ >= 0 || signal_ != 0) {
			SignalComponent* pSignal = new SignalComponent(a);
			if(signalSendId_) {
				pSignal->setSendId(signalSendId_);
			}
			if(signal_ >= 0) {
				pSignal->setRecieveId(signalRecieveMask_, signal_);
			}
		}

		a->setPickable(isPickable_);
		a->setDefaultPhysics(physics_);
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

		PhysicsComponent* pPhysics = PhysicsComponent::get(*a);
		pPhysics->nextMove() = move_;

		StatComponent* pStats = StatComponent::get(*a);
		pStats->abilities()->setBases(&abilities_);

		PosComponent::Ptr pPos(*a);
		for(int i = 0; i < Pos::MAX_ANIMS; ++i) {
			pPos->nextPos().anim(i).setAnim( anim_[i] );
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
	::setAbilities(short speed, short attack, short defense, short level) {
		short bases[4];
		bases[0] = speed;
		bases[1] = attack;
		bases[2] = defense;
		bases[3] = level;

		abilities_.setBases(bases);
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

	void SimpleActorFactory
	::setMass(scale_t m) {
		move_.mass_ = m * SCALE_RES;
	}


	void SimpleActorFactory
	::setFriction(float linear, float angular, float bounceDecay) {
		move_.linearFriction_ = linear * SCALE_STEP;
		move_.angularFriction_ = angular * SCALE_STEP;
		move_.bounceMaintain_ = 1 - bounceDecay;
	}


	void SimpleActorFactory
	::setSendSignal(int id) {
		signalSendId_ = id;
	}


	void SimpleActorFactory
	::setRecieveSignal(unsigned long mask, const char* signal) {
		signalRecieveMask_ = mask;
		signal_ = SimSchema::sortedSimObjectList().signal(signal);

	}

	void SimpleActorFactory
	::setAnim(int id, int movementMode, float pos, float speed) {
		Assert(id >= 0 && id < Pos::MAX_ANIMS);
		anim_[id].setMovementMode(id);
		anim_[id].setStartPos(pos);
		anim_[id].resetPos();
		anim_[id].setSpeed(speed);
	}
}
