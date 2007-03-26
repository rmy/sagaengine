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
#include "util/error/Log.hpp"
#include "sim/schema/SimSchema.hpp"
#include "comp/schema/CompSchema.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {

	AreaManager
	::AreaManager() 
			: areaCount_(0), factoryCount_(0), activeCount_(0)
			, maxWidth_(0), maxHeight_(0) {
		areas_ = new Area*[ MAX_ELEMENTS ];
		factories_ = new const AreaFactory*[ MAX_FACTORIES ];
		active_ = new Area*[ MAX_ACTIVE ];
		shouldKeep_ = new bool[ MAX_ACTIVE ];
		LogDetail("Creating AreaManager");
	}


	AreaManager
	::~AreaManager() {
		resetAll();
		delete[] areas_;
		delete[] active_;
		delete[] factories_;
		delete[] shouldKeep_;
		LogDetail("Destroying AreaManager");
	}


	void AreaManager
	::addArea(Area* area) {
		Assert(areaCount_ < MAX_ELEMENTS && "Too many elements. Adjust MAX_ELEMENTS.");

		if(area->width() > maxWidth_)
			maxWidth_ = area->width();
		if(area->height() > maxHeight_)
			maxHeight_ = area->height();


		// Init neighbours
		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[i]->isNeighbour( *area )) {
				areas_[i]->addNeighbour( area );
			}

			if(area->isNeighbour( *areas_[i] )) {
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
		LogFatal("Couldn't find area: " << name);
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
	::areaById(Composite::id_type id) {
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
			if(area == active_[i]) {
				shouldKeep_[i] = true;
				return;
			}
		}

		active_[ activeCount_ ] = area;
		active_[ activeCount_ ]->setParent(CompSchema::activeRoot());
		shouldKeep_[ activeCount_ ] = true;
		++activeCount_;

		DebugExec(integrity());
	}


	void AreaManager
	::setActive(Area* area, int pages) {
		for(int i = 0; i < activeCount_; ++i)
			shouldKeep_[i] = false;

		setActive(area);
		if(pages > 0) {
			_setActive(area, pages);
		}

		/*
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
					a->setParent(CompSchema::activeRoot());
					++activeCount_;
				}
				
				// Should keep it as active
				shouldKeep_[ index ] = true;
			}
		}
		*/

		DebugExec(integrity());

		// Remove inactive
		for(int i = 0; i < activeCount_; ++i) {
			Assert(active_[i]);
			if(!shouldKeep_[i]) {
				Area* a = active_[i];
				Assert(a);
				a->setParent(CompSchema::inactiveRoot());

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
	::_setActive(Area* area, int pages) {
		ZoneAreaComponent::Ptr aZone(*area);
		ComponentList::Iterator it(aZone->links());
		while(it.hasNext()) {
			Area* a = static_cast<Area*>(it.next().owner());
			// Already did self
			if(a == area)
				continue;

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
				a->setParent(CompSchema::activeRoot());
				++activeCount_;
			}
			
			// Should keep it as active
			shouldKeep_[ index ] = true;

			if(pages > 1) {
				_setActive(a, pages - 1);
			}

		}
	}


	void AreaManager
	::setInactive(Area* area) {
		for(int i = 0; i < activeCount_; ++i) {
			if(area == active_[i]) {
				area->setParent(CompSchema::inactiveRoot());

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
	::createArea(const char* areaName, const char* factoryName, int pageX, int pageY, int pageZ, int gridId) {
		char* name = new char[strlen(areaName) + 1];
		strcpy(name, areaName);
		//LogDetail("Created area: " << name);

		const AreaFactory* f = factory(factoryName);
		Area* a = f->create(new String(name), pageX, pageY, pageZ, gridId);
		// Needed for proper destruction
		a->setFactory(f);


		PosComponent::Ptr aPos(*a);

		// Update world coordinate
		aPos->updateWorldViewPoint();

		// Make sure both nextPos() and pos() are good
		aPos->flip();


		// Store it away
		addArea(a);

		return a;
	}


	void AreaManager
	::resetThings() {
		for(int i = 0; i < areaCount_; ++i) {
			areas_[ i ]->reset();
		}
		LogDetail("Destoryed things");
	}


	void AreaManager
	::resetAll() {
		resetThings();

		for(int i = 0; i < areaCount_; ++i) {
			if(areas_[i]->factory())
				areas_[i]->factory()->release(areas_[i]);
			else
				delete areas_[ i ];
		}
		areaCount_ = 0;
		LogDetail("Destroyed areas");

		for(int i = 0; i < factoryCount_; ++i) {
			delete factories_[i];
		}
		factoryCount_ = 0;
		LogDetail("Destroyed area factories");
	}


	void AreaManager
	::dump() {
		puts("Dumping repository...");
		for(int i = 0; i < areaCount_; ++i) {
			printf("%d - %s\n", (size_t)areas_[i]->id(), areas_[i]->name());
		}
	}


	void AreaManager
	::integrity() {
		for(int i = 0; i < activeCount_ - 1; ++i) {
			for(int j = i + 1; j < activeCount_; ++j) {
				if(active_[i] == active_[j]) {
					LogFatal(i << " == " << j);
				}
			}
		}
	}


}
