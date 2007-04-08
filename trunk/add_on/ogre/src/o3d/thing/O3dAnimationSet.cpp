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


#include "O3dPre.hpp"
#include "O3dAnimationSet.hpp"
#include "O3dAnimation.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "sim/pos/Pos.hpp"
#include "sim/pos/Anim.hpp"
#include "sim/stat/Dictionary.hpp"
#include <cstring>

using namespace se_core;

namespace se_ogre {
	O3dAnimationSet
	::O3dAnimationSet() {
		for(int i = 0; i < MAX_ANIMS; ++i) 
			animations_[ i ] = 0;
	}

	O3dAnimationSet
	::~O3dAnimationSet() {
		for(int i = 0; i < MAX_ANIMS; ++i) 
			delete animations_[ i ];
	}

	O3dAnimation* O3dAnimationSet
	::animation(const char* name) {
		int id = SimSchema::dictionary().id(DE_MOVEMENT_MODE, name);
		return animation(id);
	}


	O3dAnimation* O3dAnimationSet
	::animation(int id) {
		return animations_[id];
	}


	O3dAnimation* O3dAnimationSet
	::createAnimation(int id) {
		WasHere();
		Assert(id < MAX_ANIMS);
		Assert(animations_[id] == 0);
		animations_[id] = new O3dAnimation();
		animations_[id]->id_ = id;
		return animations_[id];
	}

}
