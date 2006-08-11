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


#ifndef se_ogre_hpp
#define se_ogre_hpp

#include "../src/o3d/all.hpp"
#include "../src/o3d/area/all.hpp"
#include "../src/o3d/config/all.hpp"
#include "../src/o3d/error/all.hpp"
#include "../src/o3d/event/all.hpp"
#include "../src/o3d/init/all.hpp"
#include "../src/o3d/input/all.hpp"
#include "../src/o3d/io/all.hpp"
#include "../src/o3d/schema/all.hpp"
#include "../src/o3d/system/all.hpp"
#include "../src/o3d/thing/all.hpp"
#include "../src/o3d/widget/all.hpp"


namespace se_ogre {

	inline bool initSeModule_Ogre() {
		//return O3dSchema::init();
		return true;
	}


	inline void cleanupSeModule_Ogre() {
		//O3dSchema::cleanup();
	}

}



#endif
