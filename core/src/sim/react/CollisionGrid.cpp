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


#include "CollisionGrid.hpp"
#include "comp/Composite.hpp"
#include "CollisionComponent.hpp"
#include "util/error/Log.hpp"
#include "util/vecmath/Point3.hpp"
#include <cstdio>


namespace se_core {
	CollisionGrid
	::CollisionGrid(coor_tile_t width, coor_tile_t height, short depth)
			: xOffset_(0), zOffset_(0), depth_(depth) {

		// root node size is whichever is greatest of width and height
		rootNodeSize_ = (width > height) ? width : height;

		// Force rootNodeSize_ to be n^2 (divides become shifts)
		rootNodeShift_ = 1;
		while((1L << rootNodeShift_) < rootNodeSize_)
			++rootNodeShift_;
		rootNodeSize_ = 1L << rootNodeShift_;

		// Precalc for fast level searches
		halfRootNodeSize_ = shiftRight(rootNodeSize_, 1);

		// No point making grid for things with size less than 1.
		while(nodeSize(depth_) == 0)
			--depth_;

		// How big should the node array be?
		totalNodeCount_ = 0;
		for(short i = 0; i < depth_; ++i) {
			totalNodeCount_ += 1 << (i * 2);
		}

		// Create node array
		nodes_ = new CollisionGridCollisionComponentList::iterator_type[ totalNodeCount_ ];
		// Init nodes to empty
		for(int i = 0; i < totalNodeCount_; ++i) {
			nodes_[i] = CollisionGridCollisionComponentList::end();
		}

		// Create and init array with pointers to
		// beginning of each node level
		nodeLevels_ = new CollisionGridCollisionComponentList::iterator_type*[ depth_ ];
		int nodePos = 0;
		for(short i = 0; i < depth_; ++i) {
			nodeLevels_[ i ] = &nodes_[ nodePos ];
			nodePos += 1 << (i * 2);
		}

	}


	CollisionGrid
	::~CollisionGrid() {
		delete[] nodes_;
		delete[] nodeLevels_;
	}


	void CollisionGrid
	::setSize(int width, int height) {
		rootNodeSize_ = (width > height) ? width : height;
		Assert(indexAtLevel(width - 1, height - 1, depth_ - 1) < totalNodeCount_);
	}


	void CollisionGrid
	::setOffset(const Point3& c) {
		xOffset_ = c.xTile();
		zOffset_ = c.zTile();
	}


	void CollisionGrid
	::insert(const Point3& c, coor_t size, CollisionComponent& thing) {
		const coor_tile_t x = c.xTile() - xOffset_;
		const coor_tile_t z = c.zTile() - zOffset_;
		short level = calcLevel(CoorT::tile(size) + 1);

		AssertFatal(indexAtLevel(x, z, level) >= 0, thing.owner()->name());
		AssertFatal(indexAtLevel(x, z, level) < totalNodeCount_, thing.owner()->name());
		AssertFatal(x >= 0 || x < rootNodeSize_, thing.owner()->name());
		AssertFatal(z >= 0 || z < rootNodeSize_, thing.owner()->name()); 

		Assert(x < rootNodeSize_);
		Assert(z < rootNodeSize_);

		// Find the right level

		// Reference to first element pointer. Element pointer may change.
		Assert(indexAtLevel(x, z, level) >= 0);
		Assert(indexAtLevel(x, z, level) < totalNodeCount_);
		CollisionGridCollisionComponentList::iterator_type& it
			= nodeLevels_[ level ][ indexAtLevel(x, z, level) ];

		// Insert the element. The element will bed inserted at
		// the beginning of the list. Thus often moving elements
		// will stay in the beginning of node lists
		thingList().add(&thing, it);

		Assert(nodeLevels_[ level ][ indexAtLevel(x, z, level) ] !=
			   CollisionGridCollisionComponentList::end());
	}


	void CollisionGrid
	::clear() {
		for(int i = 0; i < totalNodeCount_; ++i) {
			thingList().removeChain(nodes_[i]);
		}
	}


	bool CollisionGrid
	::remove(const Point3& c, coor_t size, CollisionComponent& thing) {
		const coor_tile_t x = c.xTile() - xOffset_;
		const coor_tile_t z = c.zTile() - zOffset_;
		const coor_tile_t s = CoorT::tile(size) + 1;

		// Find the right level
		short level = calcLevel(s);

		// Reference to first element pointer. Element pointer may change.
		int index = indexAtLevel(x, z, level);
		if(index < 0) {
			LogDetail(c << " - " << size);
		}
		Assert(indexAtLevel(x, z, level) >= 0);
		Assert(indexAtLevel(x, z, level) < (1L << level) * (1L << level));

		CollisionGridCollisionComponentList::iterator_type& it = nodeLevels_[level][index];

		return thingList().remove(&thing, it);
	}


	void CollisionGrid
	::move(const Point3& from, coor_t oldSize
		   , const Point3& to, coor_t newSize
		   , CollisionComponent& thing) {

		const coor_tile_t os = CoorT::tile(oldSize) + 1;
		const coor_tile_t ox = from.xTile() - xOffset_;
		const coor_tile_t oz = from.zTile() - zOffset_;

		const coor_tile_t ns = CoorT::tile(newSize) + 1;
		const coor_tile_t nx = to.xTile() - xOffset_;
		const coor_tile_t nz = to.zTile() - zOffset_;

		int oldLevel = calcLevel(os);
		int oldIndex = indexAtLevel(ox, oz, oldLevel);
		int newLevel = (os != ns) ? calcLevel(ns) : oldLevel;
		int newIndex = indexAtLevel(nx, nz, newLevel);

		// If still in same cell, don't move
		if(oldIndex == newIndex)
			return;

		// Different cell, move
		CollisionGridCollisionComponentList::iterator_type& oldIt
			= nodeLevels_[oldLevel][ oldIndex ];
		bool didDelete = thingList().remove(&thing, oldIt);
		CollisionGridCollisionComponentList::iterator_type& newIt
			= nodeLevels_[newLevel][ newIndex ];
		thingList().add(&thing, newIt);
		return;
	}


	short CollisionGrid
	::collisionCandidates(const Point3& c, coor_t size, CollisionComponent* things[], short max) {
		// Calculate bounds to check inside
		coor_tile_t xFrom = CoorT::tile(c.x_ - size) - xOffset_;
		coor_tile_t zFrom = CoorT::tile(c.z_ - size) - zOffset_;
		coor_tile_t xTo = CoorT::tile(c.x_ + size) + 1 - xOffset_;
		coor_tile_t zTo = CoorT::tile(c.z_ + size) + 1 - zOffset_;

		// Number of candidates in array
		short count = 0;

		// Check for collision canditates in all levels
		for(short level = 0; level < depth_; ++level) {
			DbgAssert(rootNodeShift_ - level > 0 || "Depth too high");

			// For subtracting / adding loose bounds
			coor_tile_t hns = halfNodeSize(level);

			// Translate bounds coors to level grid coor
			coor_tile_t xNodeFrom = shiftRight((-hns + xFrom), (rootNodeShift_ - level));
			coor_tile_t xNodeTo = shiftRight((hns + xTo), (rootNodeShift_ - level));
			coor_tile_t zNodeFrom = shiftRight((-hns + zFrom), (rootNodeShift_ - level));
			coor_tile_t zNodeTo = shiftRight((hns + zTo), (rootNodeShift_ - level));

			// Must do on every level because loose bounds
			// differs from one level to next
			if(xNodeFrom < 0) xNodeFrom = 0;
			if(xNodeTo >= (1L << level)) xNodeTo = (1L << level) - 1;
			if(zNodeFrom < 0) zNodeFrom = 0;
			if(zNodeTo >= (1L << level)) zNodeTo = (1L << level) - 1;

			// Iterate through all nodes that are inside the bounds
			for(coor_tile_t z = zNodeFrom; z <= zNodeTo; ++z) {
				for(coor_tile_t x = xNodeFrom; x <= xNodeTo; ++x) {
					// The first element pointer of this node.
					DbgAssert(indexAtLevelAndNode(x, z, level) >= 0);
					DbgAssert(indexAtLevelAndNode(x, z, level) < totalNodeCount_);
					CollisionGridCollisionComponentList::iterator_type it
						= nodeLevels_[level][ indexAtLevelAndNode(x, z, level) ];

					// Add members of node to the candidates list
					while(it != CollisionGridCollisionComponentList::end()) {
						Assert(count < max - 1);
						things[ count++ ] =
							thingList().next(it);
					}
				}
			}
		}

		Assert(count < max);
		return count;
	}


	bool CollisionGrid
	::find(CollisionComponent& thing) {
		/*
		for(int level = 0; level < depth_; ++ level) {
			for(int z = 0; z < (1L << level); ++z) {
				for(int x = 0; x < (1L << level); ++x) {
					int i = indexAtLevelAndNode(x, z, level);
					CollisionGridThingList::iterator_type it = nodeLevels_[level][i];
					if(thingList().hasElement(thing, it)) {
						thingList().remove(thing, nodeLevels_[level][i]);
						int i1 = indexAtLevel
							(thing.pos().x_
							 , thing.pos().z_
							 , calcLevel(thing.pos().radius()));
						int i2 = indexAtLevel
							(thing.nextPos().x_
							 , thing.nextPos().z_
							 , calcLevel(thing.pos().radius()));
						LogDetail
							((sprintf
							  (
							   log_msg(), "%s(%d): rad(%d), pos %d(%d, %d), nextPos %d(%d, %d)"
							   , thing.name(), i
							   , thing.pos().radius()
							   , i1, thing.pos().x_, thing.pos().z_
							   , i2, thing.nextPos().x_, thing.nextPos().z_
							   ), log_msg()));
						return true;
					}

				}
			}
		}
		*/

		for(int i = 0; i < totalNodeCount_; ++i) {
			CollisionGridCollisionComponentList::iterator_type it = nodes_[i];
			if(thingList().hasElement(&thing, it)) {
				thingList().remove(&thing, nodes_[i]);
				return true;
			}
		}

		return false;
	}

}
