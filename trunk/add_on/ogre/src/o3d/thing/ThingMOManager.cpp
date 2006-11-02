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
#include "ThingMultiMOFactory.hpp"
#include "ThingLightFactory.hpp"
#include "ThingBillboardFactory.hpp"
#include "ThingStaticGeometryFactory.hpp"
#include "O3dThingComponent.hpp"

namespace se_ogre {

	ThingMOManager
	::ThingMOManager() : infoCount_(0), factoryCount_(0) {
		infoList_ = new ThingMOInfoList*[100];
		factories_ = new ThingMOFactory*[100];

		static ThingMovableObjectFactory tmofMovableObject("default");
		static ThingMovableObjectFactory tmofParticleSystem("ParticleSystem");
		static ThingEntityFactory tmofEntity;
		static ThingLightFactory tmofLight;
		static ThingBillboardFactory tmofBillboard;
		static ThingStaticGeometryFactory tmofStaticGeometry;
		static ThingMultiMOFactory tmofMultiMO;
	}


	ThingMOManager
	::~ThingMOManager() {
		while(infoCount_ > 0) {
			--infoCount_;
			delete infoList_[ infoCount_ ];
		}
		delete[] infoList_;
		delete[] factories_;
	}

	/*
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
		LogMsg("Added meshinfo for: " << info->thingType_);
	}
	*/


	void ThingMOManager
	::addInfoList(ThingMOInfoList* infoList) {
		// Insert - keep sorted
		int i = infoCount_;
		while(i > 0 && infoList->thingType_.compare(infoList_[i - 1]->thingType_) < 0) {
			infoList_[ i ] = infoList_[i - 1];
			--i;
		}
		infoList_[i] = infoList;
		++infoCount_;
		LogMsg("Added meshinfo for: " << infoList->thingType_);
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
		LogMsg(thingType);
		int index = infoIndex(thingType);
		if(index < 0) {
			LogMsg("Couldnt model for " << thingType);
			return 0;
		}
		LogMsg("Moeld count for " << thingType << " is " << infoList_[index]->infoCount_);
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
	::addFactory(ThingMOFactory* factory) {
		// Insert - keep sorted
		int i = factoryCount_;
		while(i > 0 && factory->type().compare(factories_[i - 1]->type()) < 0) {
			factories_[ i ] = factories_[i - 1];
			--i;
		}
		factories_[i] = factory;
		++factoryCount_;
		LogMsg("Added factory for movable object type: " << factory->type());
	}


	int ThingMOManager
	::factoryIndex(const char* moType) const {
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


	O3dThingComponent* ThingMOManager
	::create(se_core::PosNode& t) {
		const ThingMOInfoList* inf = infoList(t.name());
		if(!inf || inf->infoCount_ < 1)
			return 0;

		/*
		if(inf->infoCount_ == 1) {
			const char* factoryType = inf->infos_[0]->movableObjectType_.get();
			const ThingMOFactory* f = factory(factoryType);
			if(!f) {
				LogWarning("Movable object factory type " << factoryType << " does not exist for " << t.name());	
				return 0;
			}
			return f->create(t, *inf->infos_[0]);
		}

		static ThingMOInfo dummy;
		*/
		
		//ThingMultiMO* parent = static_cast<ThingMultiMO*>(factory("multi")->create(t, dummy));
		O3dThingComponent* parent = new O3dThingComponent(static_cast<se_core::Actor*>(&t));
		for(int i = 0; i < inf->infoCount_; ++i) {
			const char* factoryType	= inf->infos_[i]->movableObjectType_.get();
			const ThingMOFactory* f = factory(factoryType);
			if(!f) {
				LogWarning("Movable object factory type " << factoryType << " does not exist for " << t.name());	
				continue;
			}
			ThingMO* child = f->create(t, *inf->infos_[i]);
			parent->add(*child);
		}
		return parent;
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

