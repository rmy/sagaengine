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


#include "PCThingParserModule.hpp"
#include "../schema/PCSchema.hpp"
#include "../thing/MeshOfThing.hpp"
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "util/type/String.hpp"
#include "util/type/TmpString.hpp"
#include "util/error/Log.hpp"
#include "sim/config/sim_config.hpp"
#include "sim/pos/Anim.hpp"
#include "sim/pos/Pos.hpp"
#include <cstring>
#include <cstdio>


using namespace se_core;


namespace se_ncurs {
	PCThingParserModule
	::PCThingParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::OGRE, ParserModule::THING, 1) {
	}


	void PCThingParserModule
	::parse(InputStream& in) {
		String* name = 0;
		String* mesh = 0;
		String* material = 0;
		String** animations = new String*[ Anim::MOVEMENT_MODE_COUNT ];
		float* animationSpeeds = new float[ Anim::MOVEMENT_MODE_COUNT ];
		String** materials = new String*[ Anim::MOVEMENT_MODE_COUNT ];
		bool doScaleByRadius = false;
		float scale = 1.0f;
		for(short i = 0; i < Anim::MOVEMENT_MODE_COUNT; ++i) {
			animations[ i ] = 0;
			animationSpeeds[ i ] = 0;
			materials[ i ] = 0;
		}

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
			default:
				LogFatal("Unsupported code!");
			}
		}

		Assert(name);
		Assert(mesh);
		PCSchema::meshOfThing.add(name, mesh, material, doScaleByRadius, scale, animations, animationSpeeds, materials);
	}
}

