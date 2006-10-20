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
#include "sim/pos/Anim.hpp"
#include "sim/pos/Pos.hpp"
#include <cstring>

using namespace se_core;

namespace se_ncurs {
	MeshOfThing
	::~MeshOfThing() {
		while(meshCount_ > 0) {
			--meshCount_;
			delete thingNames_[ meshCount_ ];
			delete meshes_[ meshCount_ ];
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
		  , String* meshName
		  , String* defaultMaterialName
		  , bool doScaleByRadius
		  , float scale
		  , String** animations
		  , float* animationSpeeds
		  , String** animationMaterials) {
		Assert(meshCount_ < MAX_MESH_COUNT);
		Assert(thingName);
		Assert(meshName);
		// TODO: Should sort...
		thingNames_[ meshCount_ ] = thingName;
		meshes_[ meshCount_ ] = meshName;
		defaultMaterials_[ meshCount_ ] = defaultMaterialName;
		scales_[ meshCount_ ] = scale;
		doScaleByRadius_[ meshCount_ ] = doScaleByRadius;
		animations_[ meshCount_ ] = animations;
		animationSpeeds_[ meshCount_ ] = animationSpeeds;
		materials_[ meshCount_ ] = animationMaterials;
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
	::mesh(short index) {
		Assert(index >= 0 && index < meshCount_);
		if(meshes_[index])
			return meshes_[index]->get();
		return 0;
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
