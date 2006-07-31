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


#include "MeshOfThing.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/Pos.hpp"
#include "sim/stat/Anim.hpp"
#include <cstring>

using namespace se_core;

namespace se_ogre {
	MeshOfThing
	::~MeshOfThing() {
		while(meshCount_ > 0) {
			--meshCount_;
			delete thingNames_[ meshCount_ ];
			delete factories_[ meshCount_ ];
			delete params_[ meshCount_ ];
			for(int i = 0; i < Anim::MOVEMENT_MODE_COUNT; ++i) {
				delete animations_[ meshCount_ ][ i ];
				delete materials_[ meshCount_ ][ i ];
			}
			delete[] animations_[ meshCount_ ];
			delete[] materials_[ meshCount_ ];
		}
	}


	void MeshOfThing
	::add(String* thingName
		  , se_core::String* factoryName
		  , Ogre::NameValuePairList* params
		  , String* defaultMaterialName
		  , bool doScaleByRadius
		  , float scale
		  , String** animations
		  , float* animationSpeeds
		  , String** animationMaterials
		  , float meshOut
		  , float billboardIn) {
		Assert(meshCount_ < MAX_MESH_COUNT);
		Assert(thingName);
		Assert(factoryName);
		// TODO: Should sort...
		thingNames_[ meshCount_ ] = thingName;
		factories_[ meshCount_ ] = factoryName;
		params_[ meshCount_ ] = params;
		defaultMaterials_[ meshCount_ ] = defaultMaterialName;
		scales_[ meshCount_ ] = scale;
		doScaleByRadius_[ meshCount_ ] = doScaleByRadius;
		animations_[ meshCount_ ] = animations;
		animationSpeeds_[ meshCount_ ] = animationSpeeds;
		materials_[ meshCount_ ] = animationMaterials;
		meshOut_[ meshCount_ ] = meshOut;
		billboardIn_[ meshCount_ ] = billboardIn;
		++meshCount_;
	}


	short MeshOfThing
	::index(const char* thingName) {
		for(int i = 0; i < meshCount_; ++i) {
			if(strcmp(thingName, thingNames_[i]->get()) == 0) {
				return i;
			}
		}
		return -1;
	}


	const char* MeshOfThing
	::factory(short index) {
		Assert(index >= 0 && index < meshCount_);
		if(factories_[index])
			return factories_[index]->get();
		return 0;
	}


	Ogre::NameValuePairList* MeshOfThing
	::params(short index) {
		Assert(index >= 0 && index < meshCount_);
		return params_[index];
	}


	float MeshOfThing
	::animationSpeed(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		return animationSpeeds_[index][animId];
	}


	const char* MeshOfThing
	::defaultMaterial(short index) {
		Assert(index >= 0 && index < meshCount_);

		if(defaultMaterials_[index])
			return defaultMaterials_[index]->get();
		return 0;
	}


	const char* MeshOfThing
	::material(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		if(materials_[index][animId])
			return materials_[index][animId]->get();
		return 0;
	}


	float MeshOfThing
	::scale(short index) {
		Assert(index >= 0 && index < meshCount_);
		return scales_[index];
	}


	float MeshOfThing
	::meshOut(short index) {
		Assert(index >= 0 && index < meshCount_);
		return meshOut_[index];
	}


	float MeshOfThing
	::billboardIn(short index) {
		Assert(index >= 0 && index < meshCount_);
		return billboardIn_[index];
	}


	bool MeshOfThing
	::doScaleByRadius(short index) {
		Assert(index >= 0 && index < meshCount_);
		return doScaleByRadius_[index];
	}


	const char* MeshOfThing
	::animation(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		if(animations_[index][animId]) {
			return animations_[index][animId]->get();
		}
		return 0;
	}

}
