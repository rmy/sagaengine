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


#ifndef o3d_config_o3d_config_hpp
#define o3d_config_o3d_config_hpp

#include "util/config/util_config.hpp"

#ifndef SE_CONFIG_CUSTOM_O3D

namespace se_ogre {
	const float WORLD_SCALE = 1.0f;
	const float WORLD_SCALE_Y = 1.0f;

	const int PAGE_DELTA = 0;

	const float TICKS_PER_SECOND = 1024.0;

	const bool IS_CONSOLE_ENABLED = false;
}
#endif

#endif
