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


#ifndef o3d_thing_ThingMOManager_hpp
#define o3d_thing_ThingMOManager_hpp

#include "O3dPre.hpp"
#include "o3d_thing.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"


namespace se_ogre {
	class _SeOgreExport ThingMOManager {
	public:
		ThingMOManager();
		~ThingMOManager();

		void addInfoList(ThingMOInfoList* infoList);
		//void addInfo(ThingMOInfo* info);
		int infoIndex(const char* thingType) const;
		const ThingMOInfo* info(int index) const;
		const ThingMOInfo* info(const char* thingType) const;
		const ThingMOInfoList* infoList(const char* thingType) const;

		void addFactory(const ThingMOFactory* factory);
		int factoryIndex(const char* moType) const;
		const ThingMOFactory* factory(const char* thingType) const;

		void create(O3dThingComponent* parent);
		void release(ThingMO* tmo);
		void release(O3dThingComponent* tc);

		void reset();

	private:
		static const int MAX_FACTORIES = 100;
		static const int MAX_INFOS = 100;
		ThingMOInfoList** infoList_;
		int infoCount_;
		const ThingMOFactory** factories_;
		int factoryCount_;
	};

}

#endif
