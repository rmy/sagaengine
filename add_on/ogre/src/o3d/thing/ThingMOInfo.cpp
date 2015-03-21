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
#include "O3dAnimation.hpp"
#include "O3dAnimationSet.hpp"
#include "util/error/Log.hpp"
#include "sim/pos/Anim.hpp"

using namespace se_core;

namespace se_ogre {

	ThingMOInfo
	::ThingMOInfo()
		: doScaleByRadius_(false), offset_(0, 0, 0), scale_(1.0f), popInSq_(0), popOutSq_(0), animationChannels_(0), channelCount_(0), isStatic_(0), isShadowCaster_(true) {
	}


	ThingMOInfo
	::~ThingMOInfo() {
		delete[] animationChannels_;
	}


	void ThingMOInfo
	::setAnimationChannels(int count) {
		if(animationChannels_) {
			// Seems to set twice at least once. Avoid memory leak.
			delete[] animationChannels_;
			animationChannels_ = 0;
		}
		animationChannels_ = new O3dAnimationSet[ count ];
		channelCount_ = count;
	}


	O3dAnimation* ThingMOInfo
	::createAnimation(int channel, int id) {
		if(channelCount_ == 0) {
			setAnimationChannels(channel + 1);
		}
		else {
			Assert(channel < channelCount_);
		}
		return animationChannels_[ channel ].createAnimation(id);
	}


	O3dAnimation* ThingMOInfo
	::animation(int channel, int id) const {
		return animationChannels_[ channel ].animation(id);
	}

}
