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


#include "O3dConfigParserModule.hpp"
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
	O3dConfigParserModule
	::O3dConfigParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::OGRE, ParserModule::CONFIG, 1) {
	}


	void O3dConfigParserModule
	::parse(InputStream& in) {
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'D': 
				{ // Dome
					String material;
					in.readString(name);
				}
				break;
				
			default:
				LogFatal("Unsupported code!");
			}
		}
	}

}

