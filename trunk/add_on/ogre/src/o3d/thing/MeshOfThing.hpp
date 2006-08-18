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


#ifndef o3d_thing_MeshOfThing_hpp
#define o3d_thing_MeshOfThing_hpp

#include "O3dPre.H"
#include "util/type/util_type.hpp"
#include "util/type/String.hpp"

namespace se_ogre {
	struct MeshInfo {
		MeshInfo();
		~MeshInfo();

		se_core::String thingName_;
		se_core::String factory_;
		Ogre::NameValuePairList params_;
		se_core::String defaultMaterial_;
		float scale_;
		bool doScaleByRadius_;

		// Level of Detail
		float meshOut_;
		// Level of Detail
		float billboardIn_;

		se_core::String* animations_;
		float* animationSpeeds_;
		se_core::String* materials_;
	};


	class _SeOgreExport MeshOfThing {
	public:
		MeshOfThing();
		~MeshOfThing();
		void add(MeshInfo* meshInfo);
		short index(const char* thingName);
		const char* factory(short index);
		Ogre::NameValuePairList* params(short index);
		float scale(short index);
		float billboardIn(short index);
		float meshOut(short index);
		bool doScaleByRadius(short index);
		const char* defaultMaterial(short index);
		const char* material(short index, short animId);
		const char* animation(short index, short animId);
		float animationSpeed(short index, short animId);

	private:
		static const short MAX_MESH_COUNT = 100;
		MeshInfo** meshInfo_;
		short meshCount_;
	};
}

#endif