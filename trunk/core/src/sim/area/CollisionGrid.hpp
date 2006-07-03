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


#ifndef engine_area_CollisionGrid_hpp
#define engine_area_CollisionGrid_hpp

#include "util/type/util_type.hpp"
#include "sim/sim.hpp"
#include "sim/config/sim_config.hpp"
#include "sim/thing/sim_thing.hpp"
#include "sim/stat/sim_stat.hpp"
#include "util/template/SinglyLinkedList.hpp"



namespace se_core {
	// CollisionGridThingList
	// Typedef of singly linked lists container that
	// will hold members of all singly linked lists
	// of all grid containers.
	typedef SinglyLinkedList<class Thing, MAX_GAME_OBJECTS, 243> CGTL;
	struct CollisionGridThingList : public CGTL {
		CollisionGridThingList(const char* name) : CGTL(name) {}
	};

	class CollisionGrid {
	public:
		CollisionGrid(coor_tile_t width, coor_tile_t height, short depth);
		virtual ~CollisionGrid();

		/**
		 * Set a new size for the grid. This affects the
		 * coordinates of each grid element, but does
		 * not affect the depth of the grid itself. Useful
		 * for using the same grid with several different
		 * sized areas. (Only one area at a time, of though!)
		 */
		void setSize(int width, int height);

		/**
		 * How wide a the node array is at a given
		 * level. Level 0 is an 1x1 array, level 1
		 * is a 2x2 array, level 2 is a 4x4 array,
		 * etc. This respectively gives a line offset
		 * of 1, 2 and 4.
		 */
		inline int lineOffset(short level) {
			return 1 << level;
		}

		/**
		 * Node size at a given level. The nodesize at
		 * level 0 is identical to the root node size.
		 * Elements should inserted at the highest level where
		 * the node size is still bigger than themselves.
		 */
		inline coor_tile_t nodeSize(short level) {
			return shiftRight(rootNodeSize_, level);
		}

		/**
		 * Half node size is typically used when searching for
		 * the level where a thing of a certain size belongs.
		 */
		inline coor_tile_t halfNodeSize(short level) {
			return shiftRight(halfRootNodeSize_, level);
		}

		inline int calcLevel(coor_tile_t size) {
			// Find the right level
			short level = 0;
			while((level + 1) < depth_ && halfNodeSize(level) > size)
				++level;
			return level;
		}


		/**
		 * Calculates the index in nodeLevels_[level][ index ]
		 * for a coordinate at a given level.
		 */
		inline int indexAtLevel(coor_tile_t x, coor_tile_t z, short level) {
			coor_tile_t nodeX = shiftRight(x, (rootNodeShift_ - level));
			coor_tile_t nodeZ = shiftRight(z, (rootNodeShift_ - level));
			return nodeX + nodeZ * lineOffset(level);
		}

		/**
		 * Calculates the index in nodeLevels_[level][ index ]
		 * for a coordinate at a given level.
		 */
		inline int indexAtLevelAndNode(coor_tile_t nodeX, coor_tile_t nodeZ, short level) {
			return nodeX + nodeZ * lineOffset(level);
		}

		/**
		 * Insert a thing in the collision grid.
		 */
		void insert(const Coor& c, coor_t size, Thing& thing);

		/**
		 * Remove a thing from the collision grid.
		 */
		bool remove(const Coor& c, coor_t size, Thing& thing);

		/**
		 * Move a thing in the collision grid.
		 */
		void move(const Coor& from, coor_t oldSize
				  , const Coor& to, coor_t newSize, Thing& thing);

		/**
		 * Return all things in grid that a thing with the given Coor and
		 * size may collide with in the things array. The max must
		 * be (n^2) - 1.
		 */
		short collisionCandidates(const Coor& c, coor_t size
									, Thing* things[], short max);

		bool find(Thing& thing);

		/**
		 * Delete all members from the collision grid.
		 */
		void clear();

	private:
		static coor_tile_t shiftRight(coor_tile_t value, short shift) {
			return value >> shift;
			//return value / ((coor_world_t)(1L << shift));
		}

		static coor_tile_t shiftLeft(coor_tile_t value, short shift) {
			return value << shift;
			//return value * (1L << shift);
		}

		/// A node has the same size in both directions (infinite height)
		coor_tile_t rootNodeSize_;
		short rootNodeShift_;

		/// The maximum depth of array.
		short depth_;

		/// Precalculated for fast level finding
		coor_tile_t halfRootNodeSize_;

		/// Array of all nodes. Elements contain a pointer to
		/// the first node in a singly list.
		CollisionGridThingList::iterator_type* nodes_;
		short totalNodeCount_;

		/// Array of nodes for each level.
		CollisionGridThingList::iterator_type** nodeLevels_;

		static CollisionGridThingList& thingList() {
			static CollisionGridThingList instance(__FILE__);
			return instance;
		}

	};
}

#endif
