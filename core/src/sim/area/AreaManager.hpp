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


#ifndef AreaManager_hpp
#define AreaManager_hpp

#include "sim_area.hpp"

namespace se_core {
	class AreaManager {
	public:
		AreaManager() : areaCount_(0), active_(0), collisionGrid_(0) {}
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

	private:
		CollisionGrid* collisionGrid();

		static const int MAX_ELEMENTS = 256;
		Area* areas_[MAX_ELEMENTS];
		int areaCount_;
		Area* active_;
		CollisionGrid* collisionGrid_;
	};

}

#endif
