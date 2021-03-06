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


#include "CollisionManager.hpp"
#include "CollisionAreaComponent.hpp"
#include "sim_react.hpp"
#include "../schema/SimSchema.hpp"
#include "../area/AreaManager.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"


namespace se_core {
	CollisionManager
	::CollisionManager()
			: RootComponent(sct_POS) 
			, gridCount_(0), gridPoolCount_(0) 
			, isGodModeOn_(false), contactCount_(0) {
		collisionGrids_ = new CollisionGrid*[ MAX_ACTIVE ];
		gridPool_ = new CollisionGrid*[ MAX_ACTIVE ];
		contactList_ = new Contact[ MAX_CONTACTS ];
	}


	CollisionManager
	::~CollisionManager() {
		for(int i = 0; i < gridCount_; ++i) {
			delete collisionGrids_[i];
		}
		gridCount_ = 0;
		delete[] collisionGrids_;
		delete[] gridPool_;
		delete[] contactList_;
		LogDetail("Destroyed area grids");
	}


	CollisionManager&  CollisionManager
	::singleton() {
		static CollisionManager instance;
		return instance;
	}


	void CollisionManager
	::step(long when) {
		if(isGodModeOn_)
			return;
		getContactList();
		resolveContacts();
	}


	CollisionGrid* CollisionManager
	::grabCollisionGrid() {
		// Create grid object if necessary
		if(!gridPoolCount_) {
			Assert(gridCount_ < MAX_ACTIVE);

			coor_tile_t maxWidth = SimSchema::areaManager.maxWidth();
			coor_tile_t maxHeight = SimSchema::areaManager.maxHeight();
			short d = 2;
			while((1 << (d + 1)) < maxWidth / 4 && (1 << (d + 1)) < maxHeight / 4)
				++d;

			// TODO: Fix this desperate solution for border creatures (* 2)
			// See also CollisionAreaManager
			CollisionGrid* g = new CollisionGrid(maxWidth * 2, maxHeight * 2, d);
			gridPool_[ gridPoolCount_++ ] = g;
			collisionGrids_[ gridCount_++ ] = g;
		}

		return gridPool_[ --gridPoolCount_ ];
	}


	void CollisionManager
	::releaseCollisionGrid(CollisionGrid* g) {
		g->clear();
		gridPool_[ gridPoolCount_++ ] = g;
	}


	void CollisionManager
	::getContactList() {
		contactCount_ = 0;

		NodeComponentList::Iterator it(children());
		while(it.hasNext()) {
			CollisionAreaComponent& cac = static_cast<CollisionAreaComponent&>(it.next());
			cac.getContactList(contactList_, contactCount_, MAX_CONTACTS);
		}
	}

	void CollisionManager
	::resolveContacts() {
		/*
		static int last = 0;
		if(contactCount_ != last) {
			last = contactCount_;
			LogWarning(contactCount_ << " collisions");
			for(int i = 0; i < contactCount_; ++i) {
				Contact& c = contactList_[i];
				LogWarning(c.ci1_.cc_->owner()->name() << " vs " << c.ci2_.cc_->owner()->name());
			}
		}
		*/
		for(int i = 0; i < contactCount_; ++i) {
			Contact& c = contactList_[i];
			c.ci1_.cc_->pushThing(c.ci1_, c.ci2_);
			c.ci2_.cc_->pushThing(c.ci2_, c.ci1_);
		}
	}



}
