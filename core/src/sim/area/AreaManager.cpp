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
#include "../stat/ThingIterator.hpp"
#include "../thing/Thing.hpp"
#include "../../util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {

	AreaManager
	::AreaManager() 
			: areaCount_(0), factoryCount_(0), active_(0), collisionGrid_(0) {
		areas_ = new Area*[ MAX_ELEMENTS ];
		factories_ = new const AreaFactory*[ MAX_FACTORIES ];
	}


	AreaManager
	::~AreaManager() {
		resetAll();
		delete areas_;
		delete factories_;
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


	// TODO: This should return a list of active areas_...
	Area* AreaManager
	::active() {
		return active_;
	}


	void AreaManager
	::setActive(Area* area) {
		if(area == active_) return;
		if(active()) {
			active()->resetCollisionGrid();
			active()->setActive(false);
		}
		active_ = area;
		active()->setCollisionGrid(collisionGrid());
		active()->setActive(true);
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
		LogMsg(name);

		const AreaFactory* f = factory(factoryName);
		Area* a = f->create(new String(name), pageX, pageY, pageZ);
		a->setFactory(f);
		addArea(a);
		return a;
	}


	void AreaManager
	::resetThings() {
		for(int i = 0; i < areaCount_; ++i) {
			areas_[ i ]->reset();
		}
		if(active()) {
			active()->setActive(false);
			active()->resetCollisionGrid();
			if(collisionGrid_)
				collisionGrid_->clear();
		}
		active_ = 0;
	}


	void AreaManager
	::resetAll() {
		resetThings();

		for(int i = 0; i < areaCount_; ++i) {
			//MultiSimObject& mbo = areas_[ i ]->allThings();
			delete areas_[ i ];
		}
		areaCount_ = 0;
		delete collisionGrid_;
		collisionGrid_ = 0;
	}


	CollisionGrid* AreaManager
	::collisionGrid() {
		// Create grid object if necessary
		coor_tile_t maxWidth = 1, maxHeight = 1;
		if(!collisionGrid_) {
			for(int i = 0; i < areaCount_; ++i) {
				coor_tile_t xs = areas_[i]->width();
				coor_tile_t zs = areas_[i]->height();
				if(xs > maxWidth) maxWidth = xs;
				if(zs > maxHeight) maxHeight = zs;
			}
			short d = 2;
			while((1 << (d + 1)) < maxWidth / 4 && (1 << (d + 1)) < maxHeight / 4)
				++d;

			collisionGrid_ = new CollisionGrid(maxWidth, maxHeight, d);
		}
		return collisionGrid_;
	}


	void AreaManager
	::dump() {
		puts("Dumping repository...");
		for(int i = 0; i < areaCount_; ++i) {
			printf("%d - %s\n", areas_[i]->id(), areas_[i]->name());
		}
	}
}
