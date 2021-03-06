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


#ifndef pc_thing_MeshOfThing_hpp
#define pc_thing_MeshOfThing_hpp

#include "util/type/util_type.hpp"

namespace se_ncurs {
	class MeshOfThing {
	public:
		MeshOfThing() : meshCount_(0) {}
		~MeshOfThing();
		void add(se_core::String* thingName
				 , se_core::String* meshName
				 , se_core::String* defaultMaterialName
				 , bool doScaleByRadius
				 , float scale
				 , se_core::String** animations
				 , float* animationSpeeds
				 , se_core::String** animationMaterials);
		short index(const char* thingName);
		const char* mesh(short index);
		float scale(short index);
		bool doScaleByRadius(short index);
		const char* defaultMaterial(short index);
		const char* material(short index, short animId);
		const char* animation(short index, short animId);
		float animationSpeed(short index, short animId);

	private:
		static const short MAX_MESH_COUNT = 100;
		se_core::String* thingNames_[ MAX_MESH_COUNT ];
		se_core::String* meshes_[ MAX_MESH_COUNT ];
		se_core::String* defaultMaterials_[ MAX_MESH_COUNT ];
		float scales_[ MAX_MESH_COUNT ];
		bool doScaleByRadius_[ MAX_MESH_COUNT ];
		se_core::String** animations_[ MAX_MESH_COUNT ];
		float* animationSpeeds_[ MAX_MESH_COUNT ];
		se_core::String** materials_[ MAX_MESH_COUNT ];
		short meshCount_;
	};
}

#endif
