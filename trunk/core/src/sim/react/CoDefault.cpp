/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#include "CoDefault.hpp"
#include "../thing/Actor.hpp"
#include "../action/all.hpp"



namespace se_core {

	CoDefault
	::CoDefault() : ThingCollide("Default")
				, PROPERTY_COLLIDE_SELF(Property::hash("Collide.SELF"))
	{
	}

	CoDefault
	::CoDefault(const char* name) : ThingCollide(name)
				, PROPERTY_COLLIDE_SELF(Property::hash("Collide.SELF"))
	{
	}

	bool CoDefault
	::collide(ContactInfo& pusher, const ContactInfo& target) const {
		switch(target.cc_->substance()) {
		case UNDEFINED:
			LogFatal(target.cc_->name());
		case MOBILE: // Mobile
			slideOrBounce(pusher, target);
			popAndDie(pusher, target);
			break;
		case STATIC: // Static
			slideOrRebound(pusher, target);
			popAndDie(pusher, target);
			break;
		case GAS: // Gas
			popAndDie(pusher, target);
			break;
		case PICK: // Pick
			popAndDie(pusher, target);
			break;
		}
		return false;
	}


	void CoDefault
	::bounce(ContactInfo& pusher
			  , const ContactInfo& target) const {
		PhysicsComponent::Ptr pPhysics(pusher.cc_);
		const PhysicsComponent::Ptr tPhysics(target.cc_);
		PosComponent::Ptr pPos(pusher.cc_);
		const PosComponent::Ptr tPos(target.cc_);

		// First, find the normalized vector n from the center of 
		// circle1 to the center of circle2
		const Vector3& v1 = pPhysics->move().velocity_;
		const Vector3& v2 = tPhysics->move().velocity_;

		const Point3& p1 = pusher.bouncePoint_;
		const Point3& p2 = target.bouncePoint_;

		Vector3 n;
		n.sub(p1, p2);
		n.y_ = 0;
		if(n.isZero())
			n.x_ = 1;
		n.normalize(); 

		// Find the length of the component of each of the movement
		// vectors along n. 
		// a1 = v1 . n
		// a2 = v2 . n
		float a1 = v1.dot(n);
		float a2 = v2.dot(n);

		// Using the optimized version, 
		// optimizedP =  2(a1 - a2)
		//              -----------
		//                m1 + m2
		float optimizedP = (2.0f * (a1 - a2)) / (pPhysics->move().mass_ + tPhysics->move().mass_);


		// Calculate v1', the new movement vector of circle1
		// v1' = v1 - optimizedP * m2 * n
		Vector3 v1d;
		//scale_t bounceDecay = (1 - pPhysics->move().bounceMaintain_);
		//v1d.scale(-optimizedP * tPhysics->move().mass_ * (1 - bounceDecay * .5f) , n);
		v1d.scale(-optimizedP * tPhysics->move().mass_, n);
		pPhysics->nextMove().didBounce_ = true;
		v1d.y_ = 0;
		pPhysics->nextMove().addForce(v1d);
	}


	void CoDefault
	::rebound(ContactInfo& pusher, const ContactInfo& target) const {
		////////////////////
		PhysicsComponent::Ptr pPhysics(*pusher.cc_);
		PosComponent::Ptr pPos(*pusher.cc_);

		const Point3& p1 = pusher.bouncePoint_;
		const Point3& p2 = target.bouncePoint_;
		const Vector3& v1 = pPhysics->move().velocity_;

		//LogWarning(p1.distance(p2));
		// First, find the normalized vector n from the center of 
		// circle1 to the center of circle2
		Vector3 n;
		n.sub(p1, p2);
		n.y_ = 0;
		if(n.isZero())
			return;
		n.normalize(); 

		// Find the length of the component of each of the movement
		// vectors along n. 
		// a1 = v1 . n
		// a2 = v2 . n
		float a1 = v1.dot(n);

		// Calculate v1', the new movement vector of circle1
		Vector3 v1d;
		//scale_t bounceDecay = (1 - pPhysics->move().bounceMaintain_);
		//v1d.scale(-2 * a1 * (1 - bounceDecay * .5f) , n);
		v1d.scale(-2 * a1, n);
		pPhysics->nextMove().didBounce_ = true;
		v1d.y_ = 0;
		pPhysics->nextMove().addForce(v1d);

		//haltIfGuilty(pusher, target);
	}

	void CoDefault
	::_away(ContactInfo& pusher
			  , const ContactInfo& target, coor_t speed) const {
		PhysicsComponent::Ptr pPhysics(pusher.cc_);
		const PhysicsComponent::Ptr tPhysics(target.cc_);

		const Point3& p1 = pusher.bouncePoint_;
		const Point3& p2 = target.bouncePoint_;
		Point3 aw;

		aw.sub(p2, p1);
		aw.y_ = 0;
		if(aw.isZero()) {
			aw.x_ = 1;
		}
		else {
			aw.normalize();
		}
		aw.scale(-speed);
		aw.y_ = 0;
		pPhysics->nextMove().addForce(aw);
	}


	bool CoDefault
	::slide(ContactInfo& pusher
			  , const ContactInfo& target) const {
		const coor_double_t SLIDE_THRESHOLD_SQ = 0.25f * 0.25f;
		// 
		coor_t distanceSq = pusher.bouncePoint_.xzDistanceSquared(target.bouncePoint_);
		if(distanceSq > SLIDE_THRESHOLD_SQ) {
			return false;
		}

		coor_t radSum = (pusher.radius_ + target.radius_);
		coor_t penetration = radSum - CoorT::sqrt(distanceSq);
		if(penetration < 0)
			penetration = 0;
		_away(pusher, target, penetration * penetration + 4 * COOR_STEP);

		return true;
	}


	bool CoDefault
	::popAndDie(ContactInfo& pusher
				, const ContactInfo& target) const {
		if(pop(pusher, target)) {
			pusher.cc_->owner()->scheduleForDestruction();
		}
		return false;

	}


	bool CoDefault
	::pop(ContactInfo& pusher
				, const ContactInfo& target) const {
		StatComponent::Ptr tStat(target.cc_);
		const Property* prop = tStat->property(PROPERTY_COLLIDE_SELF);
		AssertWarning(prop, "Missing property Collide.SELF in " << target.cc_->owner()->name());
		if(!prop) {
			return false;
		}

		StatComponent::Ptr pStat(pusher.cc_);
		const Property* propPusher = pStat->property(prop->hashValue());
		if(!propPusher)
			return false;

		switch(propPusher->type()) {
		case Property::PT_STRING:
		case Property::PT_STRING_LIST:
			if(_pop(*pusher.cc_->owner(), prop->hashValue())) {
				return true;
			}
			break;

		case Property::PT_ACTION:
			ActionComponent::Ptr(pusher.cc_)->planAction(CHANNEL_MOVEMENT, *propPusher->action());
			break;
		}
		return false;
	}


	bool CoDefault
	::_pop(se_core::Composite& spawner, unsigned int hash) {
		if(spawner.isDead())
			// Didn't pop - return false
			return false;

		StatComponent::Ptr pStat(spawner);
		const Property* propPop = pStat->property(hash);
		if(!propPop)
			// Didn't pop - return false
			return false;

		SpawnComponent::Ptr pSpawn(spawner);
		PosComponent::Ptr pPos(spawner);

		Composite* pop = 0;
		int c = propPop->valueCount();
		for(int i = 0; i < c; ++i) {
			pop = pSpawn->spawn(propPop->string(i), 0, 0);
			AssertFatal(pop, "Couldn't create " << propPop->string() << " for " << spawner.name());
			if(pop) {
				PosComponent* sPos = PosComponent::get(*pop);
				sPos->nextPos().anim(0).setAnim( pPos->pos().anim(0) );
				sPos->nextPos().anim(1).setAnim( pPos->pos().anim(1) );
				sPos->nextPos().anim(2).setAnim( pPos->pos().anim(2) );
				sPos->nextPos().anim(3).setAnim( pPos->pos().anim(3) );

				PhysicsComponent::Ptr pPhysics(spawner);
				PhysicsComponent::Ptr sPhysics(*pop);

				sPhysics->nextMove().setMove(pPhysics->nextMove());
			}
		}
		// Did pop - return true
		return true;
	}

	const CoDefault coDefault;
}
