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
		MeshInfo* info = new MeshInfo();

		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'N': // Name
				in.readString(info->thingName_);
				break;

			case 'M': // Mesh
				{
					String mesh;
					in.readString(mesh);
					info->factory_.set(Ogre::EntityFactory::FACTORY_TYPE_NAME.c_str());
					info->params_["mesh"] = mesh.get();
				}
				break;

			case 'B': // BillboardSet
				info->meshOut_ = in.readFloat();
				break;

			case 'S': // Scale
				info->scale_ = in.readFloat();
				break;

			case 'R': // Scale is radius
				info->doScaleByRadius_ = true;
				break;

			case 'O':
				in.readString(info->factory_);
				break;

			case 'P':
				{
					String n, v;
					in.readString(n);
					in.readString(v);
					info->params_[ n.get() ] = v.get();
				}
				break;

			case 'A': // Animation
				{
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					Assert(info->animations_[ animId ].isEmpty());
					in.readString(info->animations_[ animId ]);
					info->animationSpeeds_[ animId ] = in.readFloat();
				}
				break;

			case 'D': // Default material / texture
				{
					in.readString(info->defaultMaterial_);
				}
				break;

			case 'T': // Material / texture
				{
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					Assert(info->materials_[ animId ].isEmpty());
					in.readString(info->materials_[ animId ]);
				}
				break;

			case 'L': // Level of detail
				{
					info->billboardIn_ = in.readFloat();
					info->meshOut_ = in.readFloat();
				}
				break;

			default:
				LogFatal("Unsupported code!");
			}
		}

		Assert(!info->thingName_.isEmpty());
		Assert(!info->factory_.isEmpty() && "You must define exactly one of mesh name or factory");

		O3dSchema::meshOfThing.add(info);
	}

}

