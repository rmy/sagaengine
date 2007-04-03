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


#include "ZoneManager.hpp"
#include "ZoneAreaComponent.hpp"
#include "Exit.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/vecmath/Vector3.hpp"
#include "util/error/Log.hpp"



namespace se_core {

	ZoneAreaComponent
	::ZoneAreaComponent(Composite* owner, const ComponentFactory* factory) 
			: RootChildComponent(sct_ZONE, owner, factory), page_(0, 0, 0, -1), exits_(0) {
		// 3x3 array to hold self and neighbours
		for(short i = 0; i < MAX_NEIGHBOURS; ++i) {
			neighbours_[i] = 0;
		}
		// Add self in center
		neighbours_[ 1 + 1 * 3 + 1 * 9 ] = this;
		addLink(*this);
	}


	ZoneAreaComponent
	::~ZoneAreaComponent() {
		delete[] exits_;
	}


	void ZoneAreaComponent
	::setActive(bool state) {
		if(state) {
			NodeComponent* c = static_cast<NodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
			}
		}
		else {
			resetParent();
		}
	}


	void ZoneAreaComponent
	::offset(const ZoneAreaComponent& other, Page& dest) const {
		dest.sub(other.page(), page_);
	}


	bool ZoneAreaComponent
	::offset(const ZoneAreaComponent& other, const Vector3& size, Vector3& dest) const {
		Page rel(other.page(), page_);
		dest.x_ = rel.x_ * size.x_;
		dest.y_ = rel.y_ * size.y_;
		dest.z_ = rel.z_ * size.z_;

		return (page_.w_ >= 0 && rel.w_ != 0);
	}



	bool ZoneAreaComponent
	::addNeighbour(ZoneAreaComponent& other) {
		Page rel(other.page(), page_);
		if(!rel.isNeighbourOffset()) {
			return false;
		}

		int index = neighbourIndex(rel);
		Assert(neighbours_[ index ] == 0);
		neighbours_[ index ] = &other;
		return true;
	}


	void ZoneAreaComponent
	::addLink(ZoneAreaComponent& link) {
		links_.add(link);
	}


	int ZoneAreaComponent
	::neighbourIndex(const Page& rel) const {
		AssertFatal(rel.isNeighbourOffset(), "Clamp to neighbour offset first");
		AssertFatal(rel.w_ == 0, "Neighbours must be in the same grid");
		return (rel.x_ + 1) + 3 * (rel.y_ + 1) + 9 * (rel.z_ + 1);
	}


	ZoneAreaComponent* ZoneAreaComponent
	::neighbour(int relX, int relY, int relZ) {
		Page rel(relX, relY, relZ, 0);
		if(!rel.isNeighbourOffset()) {
			Page n;
			n.clampToNeighbourOffset(rel);
			ZoneAreaComponent* via = neighbours_[ neighbourIndex(n) ];
			if(!via)
				return 0;
			return via->neighbour(rel.x_ - n.x_, rel.y_ - n.y_, rel.z_ - n.z_);
		}
		return neighbours_[ neighbourIndex(rel) ];
	}


	const ZoneAreaComponent* ZoneAreaComponent
	::neighbour(int relX, int relY, int relZ) const {
		Page rel(relX, relY, relZ, 0);
		if(!rel.isNeighbourOffset()) {
			Page n;
			n.clampToNeighbourOffset(rel);
			const ZoneAreaComponent* via = neighbours_[ neighbourIndex(n) ];
			if(!via)
				return 0;
			return via->neighbour(rel.x_ - n.x_, rel.y_ - n.y_, rel.z_ - n.z_);
		}

		return neighbours_[ neighbourIndex(rel) ];
	}


	void ZoneAreaComponent
	::setExits(Exit* exits, int count) {
		if(exits_) {
			LogWarning("Overriding exits in: " << owner()->name());
			delete[] exits_;
			exits_ = 0;
		}

		exits_ = new Exit[ count ];
		for(int i = 0; i < count; ++i) {
			exits_[i] = exits[i];
		}
		exitCount_ = count;
	}


	Exit& ZoneAreaComponent
	::exit(int id) {
		Assert(id >= 0 && id < exitCount_);
		return exits_[id];
	}
}

