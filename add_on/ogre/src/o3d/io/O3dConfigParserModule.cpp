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
#include "io/parse/all.hpp"
#include "io/stream/all.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
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
		Assert(O3dSchema::sceneManager 
			   && "SceneManager must be created before loading ogre config file");
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'D': 
				try { // Dome
					String material;
					in.readString(material);
					float curvature = in.readFloat();
					float tiling = in.readFloat();
					
					O3dSchema::sceneManager->setSkyDome(true, material.get(), curvature, tiling);
				}
				catch(...) {
					LogMsg("Couldn't create skydome for ogre config file " << in.name());
				}
				break;
				
			case 'B': 
				{ // Box
					String material;
					in.readString(material);

					O3dSchema::sceneManager->setSkyBox(true, material.get());
				}
				break;
				
			default:
				LogFatal("Unsupported code!");
			}
		}
	}

}

