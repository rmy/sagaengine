#include "SimpleActorFactory.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/script/Script.hpp"
#include "comp/comp.hpp"

using namespace se_core;

namespace se_basic {

	SimpleActorFactory
	::SimpleActorFactory(String* name)
			: CompositeFactory(st_ACTOR, name)
			, isPickable_(false)
			, isCollideable_(false)
			, physics_(0)
			, collide_(0)
			, defaultAction_(0)
			, substance_(0)
			, debugLevel_(0)
			, spawnPointCount_(0)
			, spawnPoints_(0)
			, doObstructView_(false) {
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


	Composite* SimpleActorFactory
	::create() const {
		Composite* c = new Composite(this);
		c->setTag(tag_);
		c->setDebugLevel(debugLevel_);
		Actor* a = static_cast<Actor*>(actorFactory_.create(c));

		if(isCollideable_ || collide_) {
			CollisionComponent* cc = static_cast<CollisionComponent*>(collisionFactory_.create(c));
			cc->setCollide(collide_);
			cc->setCollideable(isCollideable_);
			cc->setSubstance(substance_);
			cc->setDoObstructView(doObstructView_);
		}

		//a->setPickable(isPickable_);
		a->setDefaultPhysics(physics_);
		a->nextPos().setBounds(bounds_);
		SpawnComponent::Ptr spawn(*c);
		if(spawnPointCount_)
			spawn->setSpawnPoints(spawnPointCount_, spawnPoints_);


		PhysicsComponent* pPhysics = PhysicsComponent::get(*c);
		pPhysics->nextMove() = move_;

		StatComponent* pStats = StatComponent::get(*c);
		pStats->abilities()->setBases(&abilities_);
		if(defaultAction_ && defaultAction_->hasAction()) {
			pStats->setDefaultAction(*defaultAction_->action(), &defaultAction_->parameter());
		}

		PosComponent::Ptr pPos(*c);
		for(int i = 0; i < Pos::MAX_ANIMS; ++i) {
			pPos->nextPos().anim(i).setAnim( anim_[i] );
		}

		createGenericComponents(c);
		createComponents(c);

		return c;
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
	::setPhysics(const char* name) {
		physics_ = Physics::lookup(name);
	}


	void SimpleActorFactory
	::setDefaultAction(const char* name) {
		defaultAction_ = new ActionAndParameter();
		defaultAction_->setAction(*Action::lookup(name));
	}


	void SimpleActorFactory
	::setCollide(const char* name) {
		collide_ = ThingCollide::lookup(name);
	}

	void SimpleActorFactory
	::setSubstance(int s) {
		substance_ = s;
	}

	void SimpleActorFactory
	::setDebugLevel(int level) {
		debugLevel_ = level;
	}

	void SimpleActorFactory
	::setDoObstructView(bool flag) {
		doObstructView_ = flag;
	}

	void SimpleActorFactory
	::setDoObstructViewDefault() {
		doObstructView_ = ((substance_ == 2 || substance_ == 3) && bounds_.maxY_ > 5 * COOR_RES);
	}

	void SimpleActorFactory
	::setSpawnPoints(int count, ViewPoint* const* spawnPoints) {
		if(count == 0 && spawnPoints_ != 0)
			return;
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
	::setAnim(int id, int movementMode, float pos, float speed) {
		Assert(id >= 0 && id < Pos::MAX_ANIMS);
		anim_[id].setMovementMode(id);
		anim_[id].setStartPos(pos);
		anim_[id].resetPos();
		anim_[id].setSpeed(speed);
	}
}
