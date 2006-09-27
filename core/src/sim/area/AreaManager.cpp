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


#include "AreaManager.hpp"
#include "Area.hpp"
#include "AreaFactory.hpp"
#include "CollisionGrid.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../thing/Thing.hpp"
#include "../../util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {

	AreaManager
	::AreaManager() 
			: areaCount_(0), factoryCount_(0), activeCount_(0), gridCount_(0), gridPoolCount_(0) {
		areas_ = new Area*[ MAX_ELEMENTS ];
		factories_ = new const AreaFactory*[ MAX_FACTORIES ];
		active_ = new Area*[ MAX_ACTIVE ];
		shouldKeep_ = new bool[ MAX_ACTIVE ];
		collisionGrids_ = new CollisionGrid*[ MAX_ACTIVE ];
		gridPool_ = new CollisionGrid*[ MAX_ACTIVE ];
		LogMsg("Creating AreaManager");
	}


	AreaManager
	::~AreaManager() {
		resetAll();
		delete[] areas_;
		delete[] active_;
		delete[] factories_;
		delete[] collisionGrids_;
		delete[] shouldKeep_;
		delete[] gridPool_;
		LogMsg("Destroying AreaManager");
	}


	void AreaManager
	::addArea(Area* area) {
		Assert(areaCount_ < MAX_ELEMENTS && "Too many elements. Adjust MAX_ELEMENTS.");

		// Init neighbours
		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[i]->addNeighbour( area )) {
				area->addNeighbour(areas_[i]);
			}
		}

		// Add area to repository
		areas_[ areaCount_++ ] = area;
	}


	Area* AreaManager
	::area(const char* name) {
		for(int i = 0; i < areaCount_; ++i) {
			if(strcmp(areas_[ i ]->name(), name) == 0) {
				return areas_[ i ];
			}
		}
		LogFatal(name);
		LogFatal("Couldn't find area");
		return 0;
	}


	int AreaManager
	::areasByFactory(const char* name, Area** dest, int maxCount) {
		int c = 0;
		for(int i = 0; i < areaCount_; ++i) {
			Assert(areas_[ i ]->factory());
			Assert(areas_[ i ]->factory()->name());
			if(strcmp(areas_[ i ]->factory()->name(), name) == 0) {
				dest[ c++ ] = areas_[ i ];
				if(c >= maxCount)
					break;
			}
		}
		return c;
	}


	bool AreaManager
	::hasArea(const char* name) {
		for(int i = 0; i < areaCount_; ++i) {
			if(strcmp(areas_[ i ]->name(), name) == 0) {
				return true;
			}
		}
		return false;
	}


	Area* AreaManager
	::area(int id) {
		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[ i ]->id() == id) {
				return areas_[ i ];
			}
		}
		LogFatal("Couldn't find area");
		return 0;
	}


	Area* AreaManager
	::active(int index) {
		Assert(index >= 0 && index < areaCount_);
		return active_[ index ];
	}


	void AreaManager
	::setActive(Area* area) {
		for(int i = 0; i < activeCount_; ++i) {
			if(area == active_[i]) return;
		}

		active_[ activeCount_ ] = area;
		active_[ activeCount_ ]->setCollisionGrid(grabCollisionGrid());
		active_[ activeCount_ ]->setActive(true);
		++activeCount_;

		DebugExec(integrity());
	}


	void AreaManager
	::setActive(Area* area, int pages) {
		for(int i = 0; i < activeCount_; ++i)
			shouldKeep_[i] = false;

		for(short relZ = -pages; relZ <= pages; ++relZ) {
			for(short relX = -pages; relX <= pages; ++relX) {
				Area* a = area->neighbour(relX, 0, relZ);
				if(!a) continue;
				
				// Already active?
				int index = -1;
				for(int i = 0; i < activeCount_; ++i) {
					if(a == active_[i]) {
						index = i;
						break;
					}
				}


				// Nope?
				if(index < 0) {
					// Make it active
					index = activeCount_;
					active_[ index ] = a;
					CollisionGrid* g = grabCollisionGrid();
					active_[ index ]->setCollisionGrid(g);
					active_[ index ]->setActive(true);
					++activeCount_;
				}
				
				// Should keep it as active
				shouldKeep_[ index ] = true;
			}
		}

		DebugExec(integrity());

		// Remove inactive
		for(int i = 0; i < activeCount_; ++i) {
			Assert(active_[i]);
			if(!shouldKeep_[i]) {
				// Get back collisionGrid to pool
				//LogMsg(i << " < " << activeCount_);
				Area* a = active_[i];
				Assert(a);
				CollisionGrid* g = a->resetCollisionGrid();
				Assert(g);
				if(g) {
					g->clear();
					gridPool_[ gridPoolCount_++ ] = g;
				}
				a->setActive(false);

				// Delete current by moving last to here
				--activeCount_;
				active_[ i ] = active_[ activeCount_ ];
				shouldKeep_[ i ] = shouldKeep_[ activeCount_ ];
				active_[ activeCount_ ] = 0;

				// Do this index again, as it now contains new area
				--i;
			}
		}

		DebugExec(integrity());

	}


	void AreaManager
	::setInactive(Area* area) {
		for(int i = 0; i < activeCount_; ++i) {
			if(area == active_[i]) {
				CollisionGrid* g = area->resetCollisionGrid();
				if(g) {
					g->clear();
					gridPool_[ gridPoolCount_++ ] = g;
				}
				area->setActive(false);

				--activeCount_;
				active_[ i ] = active_[ activeCount_ ];
				active_[ activeCount_ ] = 0;
			}
		}

		DebugExec(integrity());
	}


	void AreaManager
	::resetActive() {
		while(activeCount_ > 0) {
			setInactive(active_[0]);
		}

		DebugExec(integrity());
	}

	void AreaManager
	::addFactory(const AreaFactory* f) {
		factories_[ factoryCount_++ ] = f;
	}


	const AreaFactory* AreaManager
	::factory(const char* name) const {
		for(int i = 0; i < factoryCount_; ++i) {
			if(strcmp(factories_[ i ]->name(), name) == 0) {
				return factories_[i];
			}
		}
		LogFatal("AreaFactory '" << name << "' does not exist.");
		return 0;
	}


	Area* AreaManager
	::createArea(const char* areaName, const char* factoryName, int pageX, int pageY, int pageZ) {
		char* name = new char[strlen(areaName) + 1];
		strcpy(name, areaName);
		//LogMsg("Created area: " << name);

		const AreaFactory* f = factory(factoryName);
		Area* a = f->create(new String(name), pageX, pageY, pageZ);

		// Update world coordinate
		a->updateWorldViewPoint();

		// Make sure both nextPos() and pos() are good
		a->flip();

		// Needed for proper destruction
		a->setFactory(f);

		// Store it away
		addArea(a);

		return a;
	}


	void AreaManager
	::resetThings() {
		for(int i = 0; i < areaCount_; ++i) {
			areas_[ i ]->reset();
		}
		LogMsg("Destoryed things");
		/*
		if(active()) {
			active()->setActive(false);
			active()->resetCollisionGrid();
			if(collisionGrid_)
				collisionGrid_->clear();
		}
		*/
	}


	void AreaManager
	::resetAll() {
		resetThings();

		for(int i = 0; i < areaCount_; ++i) {
			// TODO: release through factory
			if(areas_[i]->factory())
				areas_[i]->factory()->release(areas_[i]);
			else
				delete areas_[ i ];
		}
		areaCount_ = 0;
		LogMsg("Destroyed areas");

		for(int i = 0; i < factoryCount_; ++i) {
			delete factories_[i];
		}
		factoryCount_ = 0;
		LogMsg("Destroyed area factories");

		for(int i = 0; i < gridCount_; ++i) {
			delete collisionGrids_[i];
		}
		gridCount_ = 0;
		LogMsg("Destroyed area grids");
	}


	CollisionGrid* AreaManager
	::grabCollisionGrid() {
		// Create grid object if necessary
		coor_tile_t maxWidth = 1, maxHeight = 1;
		if(!gridPoolCount_) {
			Assert(gridCount_ < MAX_ACTIVE);

			for(int i = 0; i < areaCount_; ++i) {
				coor_tile_t xs = areas_[i]->width();
				coor_tile_t zs = areas_[i]->height();
				if(xs > maxWidth) maxWidth = xs;
				if(zs > maxHeight) maxHeight = zs;
			}
			short d = 2;
			while((1 << (d + 1)) < maxWidth / 4 && (1 << (d + 1)) < maxHeight / 4)
				++d;

			CollisionGrid* g = new CollisionGrid(maxWidth, maxHeight, d);
			gridPool_[ gridPoolCount_++ ] = g;
			collisionGrids_[ gridCount_++ ] = g;
		}

		return gridPool_[ --gridPoolCount_ ];
	}


	void AreaManager
	::dump() {
		puts("Dumping repository...");
		for(int i = 0; i < areaCount_; ++i) {
			printf("%d - %s\n", areas_[i]->id(), areas_[i]->name());
		}
	}


	void AreaManager
	::integrity() {
		for(int i = 0; i < activeCount_; ++i) {
			Assert(active_[i]->collisionGrid_);
		}
		for(int i = 0; i < activeCount_ - 1; ++i) {
			for(int j = i + 1; j < activeCount_; ++j) {
				if(active_[i] == active_[j]) {
					LogFatal(i << " == " << j);
				}
			}
		}
	}


}
