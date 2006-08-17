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

	MeshInfo
	::MeshInfo() : scale_(1.0f), billboardIn_(0), meshOut_(200.0) {
		animations_ = new se_core::String[ Anim::MOVEMENT_MODE_COUNT ];
		animationSpeeds_ = new float[ Anim::MOVEMENT_MODE_COUNT ];
		materials_ = new se_core::String[ Anim::MOVEMENT_MODE_COUNT ];
		for(int i = 0; i < Anim::MOVEMENT_MODE_COUNT; ++i) {
			animationSpeeds_[i] = 0;
		}
	}


	MeshInfo
	::~MeshInfo() {
		delete[] animations_;
		delete[] animationSpeeds_;
		delete[] materials_;
	}


	MeshOfThing
	::MeshOfThing() : meshCount_(0) {
		meshInfo_ = new MeshInfo*[100];
	}


	MeshOfThing
	::~MeshOfThing() {
		while(meshCount_ > 0) {
			--meshCount_;
			delete meshInfo_[ meshCount_ ];
		}
		delete meshInfo_;
	}


	void MeshOfThing
	::add(MeshInfo* meshInfo) {
		// Insert - keep sorted
		int i = meshCount_;
		while(i > 0 && meshInfo->thingName_.compare(meshInfo_[--i]->thingName_) < 0) {
			meshInfo_[ i + 1 ] = meshInfo_[i];
		}
		meshInfo_[i] = meshInfo;
		++meshCount_;
		LogMsg("Added meshinfo for: " << meshInfo->thingName_);
	}


	short MeshOfThing
	::index(const char* thingName) {
		int min = 0;
		int max = meshCount_ - 1;

		// Binary search
		while(max >= min) {
			int middle = (min + max) << 1;
			int cmp = meshInfo_[ middle ]->thingName_.compare( thingName );
			if(cmp < 0) {
				min = middle + 1;
			}
			else if(cmp > 0) {
				max = middle - 1;
			}
			else {
				return middle;
			}
		}

		return -1;
	}


	const char* MeshOfThing
	::factory(short index) {
		Assert(index >= 0 && index < meshCount_);
		if(!meshInfo_[index]->factory_.isEmpty())
			return meshInfo_[index]->factory_.get();
		return 0;
	}


	Ogre::NameValuePairList* MeshOfThing
	::params(short index) {
		Assert(index >= 0 && index < meshCount_);
		return &meshInfo_[index]->params_;
	}


	float MeshOfThing
	::animationSpeed(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		return meshInfo_[index]->animationSpeeds_[animId];
	}


	const char* MeshOfThing
	::defaultMaterial(short index) {
		Assert(index >= 0 && index < meshCount_);

		if(!meshInfo_[index]->defaultMaterial_.isEmpty())
			return meshInfo_[index]->defaultMaterial_.get();
		return 0;
	}


	const char* MeshOfThing
	::material(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		if(!meshInfo_[index]->materials_[animId].isEmpty())
			return meshInfo_[index]->materials_[animId].get();
		return 0;
	}


	float MeshOfThing
	::scale(short index) {
		Assert(index >= 0 && index < meshCount_);
		return meshInfo_[index]->scale_;
	}


	float MeshOfThing
	::meshOut(short index) {
		Assert(index >= 0 && index < meshCount_);
		return meshInfo_[index]->meshOut_;
	}


	float MeshOfThing
	::billboardIn(short index) {
		Assert(index >= 0 && index < meshCount_);
		return meshInfo_[index]->billboardIn_;
	}


	bool MeshOfThing
	::doScaleByRadius(short index) {
		Assert(index >= 0 && index < meshCount_);
		return meshInfo_[index]->doScaleByRadius_;
	}


	const char* MeshOfThing
	::animation(short index, short animId) {
		Assert(index >= 0 && index < meshCount_);
		if(!meshInfo_[index]->animations_[animId].isEmpty()) {
			return meshInfo_[index]->animations_[animId].get();
		}
		return 0;
	}

}
