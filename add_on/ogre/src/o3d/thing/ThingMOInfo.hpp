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


#ifndef o3d_thing_ThingMOInfo_hpp
#define o3d_thing_ThingMOInfo_hpp

#include "O3dPre.hpp"
#include "o3d_thing.hpp"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"
#include <OgreCommon.h>
#include <OgreColourValue.h>

namespace se_ogre {
	struct _SeOgreExport ThingMOInfo {
		ThingMOInfo();
		~ThingMOInfo();

		// Per thing
		bool doScaleByRadius_;
		//se_core::String thingType_;

		// Per movable object
		se_core::String movableObjectType_;
		Ogre::NameValuePairList params_;
		se_core::String defaultMaterial_;

		// Offset
		Ogre::Vector3 offset_;
		Ogre::Quaternion rot_;

		// Level of Detail
		float scale_;
		float popInSq_;
		float popOutSq_;

		O3dAnimationSet* animationChannels_;
		bool isStatic_;

		bool isShadowCaster_;

		void setAnimationChannels(int count);
		O3dAnimation* animation(int channel, int id) const;
		O3dAnimation* createAnimation(int channel, int id);
		int channelCount() const { return channelCount_; }

		// Light values
		Ogre::ColourValue diffuse_, specular_;

	private:
		int channelCount_;
	};

}

#endif

