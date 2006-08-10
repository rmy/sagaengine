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


#ifndef sim_area_AreaManager_hpp
#define sim_area_AreaManager_hpp

#include "sim_area.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport AreaManager {
	public:
		AreaManager();
		~AreaManager();
		void addArea(Area* area);
		bool hasArea(const char* name);
		Area* area(const char* name);
		Area* area(int id);
		int activeCount() { return activeCount_; }
		Area* active(int index);
		void resetThings();
		void resetAll();
		void dump();
		void resetActive();
		void setActive(Area* area);
		void setActive(Area* area, int pages);
		void setInactive(Area* area);

		void addFactory(const AreaFactory* factory);
		Area* createArea(const char* areaName, const char* factoryName, int pageX = -1, int pageY = -1, int pageZ = -1);

		void integrity();

	private:
		CollisionGrid* grabCollisionGrid();

		const AreaFactory* factory(const char* name) const;


		static const int MAX_ELEMENTS = 1024 * 4;
		static const int MAX_FACTORIES = 32;
		static const int MAX_ACTIVE = 3 * (7 * 7 * 7);

		int areaCount_;
		int factoryCount_;
		int activeCount_;

		int gridCount_, gridPoolCount_;
		CollisionGrid** collisionGrids_;
		CollisionGrid** gridPool_;

		const AreaFactory** factories_;
		Area** areas_;
		Area** active_;
		bool* shouldKeep_;
	};

}

#endif
