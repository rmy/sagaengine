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
#include "ThingMultiMO.hpp"
#include "ThingMOInfo.hpp"
#include "ThingMOInfoList.hpp"
#include "ThingMOFactory.hpp"
#include "ThingMovableObjectFactory.hpp"
#include "ThingEntityFactory.hpp"
#include "ThingDebugFactory.hpp"
#include "ThingParticleFactory.hpp"
#include "ThingMultiMOFactory.hpp"
#include "ThingLightFactory.hpp"
#include "ThingBillboardFactory.hpp"
#include "ThingStaticGeometryFactory.hpp"
#include "O3dThingComponent.hpp"

using namespace se_core;

namespace se_ogre {

	ThingMOManager
	::ThingMOManager() : infoCount_(0), factoryCount_(0) {
		infoList_ = new ThingMOInfoList*[MAX_INFOS];
		factories_ = new const ThingMOFactory*[MAX_FACTORIES];

		static const ThingMovableObjectFactory tmofMovableObject("default");
		static const ThingEntityFactory tmofEntity;
		static const ThingDebugFactory tmofDebug;
		static const ThingLightFactory tmofLight;
		static const ThingBillboardFactory tmofBillboard;
		static const ThingStaticGeometryFactory tmofStaticGeometry;
		static const ThingParticleFactory tmofParticle;
		static const ThingMultiMOFactory tmofMultiMO;
	}


	ThingMOManager
	::~ThingMOManager() {
		reset();
		for(int i = 0; i < infoCount_; ++i) {
			delete infoList_[i];
		}
		delete[] infoList_;
		delete[] factories_;
	}

	/*23283940 Elin Bjelkerud
	void ThingMOManager
	::addInfo(ThingMOInfo* info) {
		// Insert - keep sorted
		int i = infoCount_;
		while(i > 0 && info->thingType_.compare(info_[i - 1]->thingType_) < 0) {
			info_[ i ] = info_[i - 1];
			--i;
		}
		info_[i] = info;
		++infoCount_;
		LogDetail("Added meshinfo for: " << info->thingType_);
	}
	*/


	void ThingMOManager
	::addInfoList(ThingMOInfoList* infoList) {
		Assert(infoCount_ < MAX_INFOS);
		// Insert - keep sorted
		int i = infoCount_;
		while(i > 0 && infoList->thingType_.compare(infoList_[i - 1]->thingType_) < 0) {
			infoList_[ i ] = infoList_[i - 1];
			--i;
		}
		infoList_[i] = infoList;
		++infoCount_;
		LogDetail("Added meshinfo for: " << infoList->thingType_);
	}


	void ThingMOManager
	::reset() {
		while(infoCount_ > 0) {
			--infoCount_;
			delete infoList_[ infoCount_ ];
		}
		infoCount_ = 0;
	}

	int ThingMOManager
	::infoIndex(const char* thingType) const {
		int min = 0;
		int max = infoCount_ - 1;

		// Binary search
		while(max >= min) {
			int middle = (min + max) >> 1;
			int cmp = infoList_[ middle ]->thingType_.compare( thingType );
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
		return infoList_[index]->infos_[0];
	}


	const ThingMOInfo* ThingMOManager
	::info(const char* thingType) const {
		LogDetail(thingType);
		int index = infoIndex(thingType);
		if(index < 0) {
			LogDetail("Couldnt model for " << thingType);
			return 0;
		}
		LogDetail("Moeld count for " << thingType << " is " << infoList_[index]->infoCount_);
		return infoList_[index]->infos_[0];
	}


	const ThingMOInfoList* ThingMOManager
	::infoList(const char* thingType) const {
		int index = infoIndex(thingType);
		if(index < 0)
			return 0;
		return infoList_[index];
	}


	void ThingMOManager
	::addFactory(const ThingMOFactory* factory) {
		Assert(factoryCount_ < MAX_FACTORIES);
		// Insert - keep sorted
		int i = factoryCount_;
		while(i > 0 && factory->type().compare(factories_[i - 1]->type()) < 0) {
			factories_[ i ] = factories_[i - 1];
			--i;
		}
		factories_[i] = factory;
		++factoryCount_;
		LogDetail("Added factory for movable object type: " << factory->type());
	}


	int ThingMOManager
	::factoryIndex(const char* moType) const {
		Assert(factories_[0]->type().get() != (const char*)(0x1));
		int min = 0;
		int max = factoryCount_ - 1;

		// Binary search
		while(max >= min) {
			int middle = (min + max) >> 1;
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


	void ThingMOManager
	::create(O3dThingComponent* parent) {
		const ThingMOInfoList* inf = infoList(parent->owner()->name());
		for(int i = 0; i < inf->infoCount_; ++i) {
			const char* factoryType	= inf->infos_[i]->movableObjectType_.get();
			const ThingMOFactory* f = factory(factoryType);
			if(!f) {
				LogWarning("Movable object factory type " << factoryType << " does not exist for " << parent->owner()->name());	
				continue;
			}
			ThingMO* child = f->create(*PosComponent::get(*parent), *inf->infos_[i]);
			Assert(child);
			parent->add(*child);
		}
	}


	void ThingMOManager
	::release(ThingMO* tmo) {
		tmo->factory_.release(tmo);
	}


	void ThingMOManager
	::release(O3dThingComponent* tc) {
		delete tc;
	}


}

