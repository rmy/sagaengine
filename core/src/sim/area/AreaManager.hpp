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
	class AreaManager {
	public:
		AreaManager();
		~AreaManager();
		void addArea(Area* area);
		bool hasArea(const char* name);
		Area* area(const char* name);
		Area* area(int id);
		Area* active();
		void resetThings();
		void resetAll();
		void dump();
		void setActive(Area* area);

		void addFactory(const AreaFactory* factory);
		Area* createArea(const char* areaName, const char* factoryName, int pageX = -1, int pageY = -1, int pageZ = -1);


	private:
		CollisionGrid* collisionGrid();

		const AreaFactory* factory(const char* name) const;


		static const int MAX_ELEMENTS = 1024;
		static const int MAX_FACTORIES = 32;

		int areaCount_;
		int factoryCount_;
		Area* active_;
		CollisionGrid* collisionGrid_;

		const AreaFactory** factories_;
		Area** areas_;
	};

}

#endif
