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


#include "ThingMOInfo.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/Anim.hpp"

using namespace se_core;

namespace se_ogre {

	ThingMOInfo
	::ThingMOInfo() : scale_(1.0f), billboardIn_(0), meshOut_(200.0) {
		animations_ = new se_core::String[ Anim::MOVEMENT_MODE_COUNT ];
		animationSpeeds_ = new float[ Anim::MOVEMENT_MODE_COUNT ];
		materials_ = new se_core::String[ Anim::MOVEMENT_MODE_COUNT ];
		for(int i = 0; i < Anim::MOVEMENT_MODE_COUNT; ++i) {
			animationSpeeds_[i] = 0;
		}
	}


	ThingMOInfo
	::~ThingMOInfo() {
		delete[] animations_;
		delete[] animationSpeeds_;
		delete[] materials_;
	}

}
