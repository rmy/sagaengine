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


#include "O3dThingParserModule.hpp"
#include "../schema/O3dSchema.hpp"
#include "../thing/MeshOfThing.hpp"
#include "../io/all.hpp"
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/type/TmpString.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/Pos.hpp"
#include "sim/stat/Anim.hpp"
#include <cstring>
#include <cstdio>


using namespace se_core;


namespace se_ogre {
	O3dThingParserModule
	::O3dThingParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::OGRE, ParserModule::THING, 1) {
	}


	void O3dThingParserModule
	::parse(InputStream& in) {
		WasHere();
		String* name = 0;
		String* mesh = 0;
		String* factory = 0;
		Ogre::NameValuePairList& params = *(new Ogre::NameValuePairList());
		String* material = 0;
		String** animations = new String*[ Anim::MOVEMENT_MODE_COUNT ];
		float* animationSpeeds = new float[ Anim::MOVEMENT_MODE_COUNT ];
		String** materials = new String*[ Anim::MOVEMENT_MODE_COUNT ];
		bool doScaleByRadius = false;
		float scale = 1.0f, billboardIn = 0.0f, meshOut = 20.0f;

		WasHere();
		for(short i = 0; i < Anim::MOVEMENT_MODE_COUNT; ++i) {
			animations[ i ] = 0;
			animationSpeeds[ i ] = 0;
			materials[ i ] = 0;
		}

		WasHere();
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'N': // Name
				name = new String();
				in.readString(*name);
				break;

			case 'M': // Mesh
				mesh = new String();
				in.readString(*mesh);
				break;

			case 'S': // Scale
				scale = in.readFloat();
				break;

			case 'R': // Scale is radius
				doScaleByRadius = true;
				break;

			case 'O':
				{
					factory = new String();
					in.readString(*factory);
				}
				break;

			case 'P':
				{
					String n, v;
					in.readString(n);
					in.readString(v);
					params[ n.get() ] = v.get();
				}
				break;

			case 'A': // Animation
				{
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					Assert(!animations[ animId ]);
					animations[ animId ] = new String();
					animationSpeeds[ animId ] = in.readFloat();
					in.readString(*animations[ animId ]);
				}
				break;

			case 'D': // Default material / texture
				{
					WasHere();
					material = new String();
					in.readString(*material);
				}
				break;

			case 'T': // Material / texture
				{
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					Assert(!materials[ animId ]);
					materials[ animId ] = new String();
					in.readString(*materials[ animId ]);
				}
				break;

			case 'B': // Billboard
				{
					billboardIn = in.readFloat();
					meshOut = in.readFloat();
				}
				break;

			case 'F': //
				{
					meshOut = in.readFloat();
				}
				break;
			default:
				LogFatal("Unsupported code!");
			}
		}
		WasHere();

		Assert(name);
		Assert(mesh || factory && !(mesh && factory) && "You must define exactly one of mesh name or factory");

		if(mesh) {
			factory = new String(Ogre::EntityFactory::FACTORY_TYPE_NAME.c_str());
			params["mesh"] = mesh->get();
		}
		O3dSchema::meshOfThing.add(name, factory, &params, material, doScaleByRadius, scale, animations, animationSpeeds, materials, meshOut, billboardIn);
	}

}

