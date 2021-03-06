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


#ifndef o3d_thing_O3dAnimationSet_hpp
#define o3d_thing_O3dAnimationSet_hpp

#include "O3dAnimation.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"

namespace se_ogre {
	class _SeOgreExport O3dAnimationSet {
	public:
		O3dAnimationSet();
		~O3dAnimationSet();
		O3dAnimation* createAnimation(int id);
		O3dAnimation* animation(const char* name);
		O3dAnimation* animation(int id);

	private:
		enum { MAX_ANIMS = 64 };
		O3dAnimation* animations_[MAX_ANIMS];
	};
}

#endif
