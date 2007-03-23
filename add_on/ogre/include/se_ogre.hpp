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

#include "../src/o3d/o3d.hpp"
#include "../src/o3d/area/o3d_area.hpp"
#include "../src/o3d/config/o3d_config.hpp"
//#include "../src/o3d/error/o3d_error.hpp"
#include "../src/o3d/event/o3d_event.hpp"
#include "../src/o3d/input/o3d_input.hpp"
#include "../src/o3d/io/o3d_io.hpp"
#include "../src/o3d/schema/O3dSchema.hpp"
#include "../src/o3d/system/o3d_system.hpp"
#include "../src/o3d/thing/o3d_thing.hpp"
#include "../src/o3d/widget/o3d_widget.hpp"


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
