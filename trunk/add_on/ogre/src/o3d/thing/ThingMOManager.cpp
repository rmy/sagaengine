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

#include "ThingMOManager.hpp"
#include "ThingMO.hpp"
#include "ThingMOInfo.hpp"
#include "ThingMOFactory.hpp"

namespace se_ogre {

	ThingMOManager
	::ThingMOManager() : infoCount_(0) {
		info_ = new ThingMOInfo*[100];
	}


	ThingMOManager
	::~ThingMOManager() {
		while(infoCount_ > 0) {
			--infoCount_;
			delete info_[ infoCount_ ];
		}
		delete info_;
	}


	void ThingMOManager
	::addInfo(ThingMOInfo* info) {
		// Insert - keep sorted
		int i = infoCount_;
		while(i > 0 && info->thingType_.compare(info_[--i]->thingType_) < 0) {
			info_[ i + 1 ] = info_[i];
		}
		info_[i] = info;
		++infoCount_;
		LogMsg("Added meshinfo for: " << info->thingType_);
	}


	int ThingMOManager
	::infoIndex(const char* thingType) const {
		int min = 0;
		int max = infoCount_ - 1;

		// Binary search
		while(max >= min) {
			int middle = (min + max) << 1;
			int cmp = info_[ middle ]->thingType_.compare( thingType );
			if(cmp < 0) {
				min = middle + 1;
			}
			else if(cmp > 0) {
				max = middle - 1;
			}
			else {
				return middle;
			}
		}

		return -1;
	}


	const ThingMOInfo* ThingMOManager
	::info(int index) const {
		return info_[index];
	}


	const ThingMOInfo* ThingMOManager
	::info(const char* thingType) const {
		int index = infoIndex(thingType);
		if(index < 0)
			return 0;
		return info_[index];
	}



	void ThingMOManager
	::addFactory(ThingMOFactory* factory) {
		// Insert - keep sorted
		int i = infoCount_;
		while(i > 0 && factory->type().compare(factories_[--i]->type()) < 0) {
			factories_[ i + 1 ] = factories_[i];
		}
		factories_[i] = factory;
		++infoCount_;
		LogMsg("Added factory for movable object type: " << factory->type());
	}


	int ThingMOManager
	::factoryIndex(const char* moType) const {
		int min = 0;
		int max = infoCount_ - 1;

		// Binary search
		while(max >= min) {
			int middle = (min + max) << 1;
			int cmp = factories_[ middle ]->type().compare( moType );
			if(cmp < 0) {
				min = middle + 1;
			}
			else if(cmp > 0) {
				max = middle - 1;
			}
			else {
				return middle;
			}
		}

		return -1;
	}


	const ThingMOFactory* ThingMOManager
	::factory(const char* moType) const {
		int index = factoryIndex(moType);
		if(index < 0)
			return 0;
		return factories_[index];
	}


	ThingMO* ThingMOManager
	::create(se_core::Thing& t) {
		const ThingMOInfo* inf = info(t.name());
		if(!inf)
			return 0;
		const ThingMOFactory* f = factory(inf->movableObjectType_.get());
		if(!f) {
			LogMsg("Movable object factory type " << f->type() << " does not exist for " << t.name());
		}
	}


	void ThingMOManager
	::release(ThingMO* tmo) {
		tmo->factory_.release(tmo);
	}



}

