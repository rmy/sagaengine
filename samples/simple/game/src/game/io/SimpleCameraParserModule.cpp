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


#include "GamePre.H"
#include "SimpleCameraParserModule.hpp"
#include "../thing/SimpleCameraFactory.hpp"

using namespace se_core;

namespace game {


	SimpleCameraParserModule
	::SimpleCameraParserModule(Parser& parser)
			: ParserModule(parser, ParserModule::GAME, ParserModule::THING, 3)  {
	}


	void SimpleCameraParserModule
	::parse(InputStream& in) {
		int code = ' ';

		code = in.readInfoCode();
		Assert(code == 'C');

		String* name = new String();
		in.readString(*name);
		SimpleCameraFactory* factory = new SimpleCameraFactory(name);

		String collide("default"); // Default collide

		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'R': // Radius
				{
					float radius = in.readFloat();
					factory->setRadius(radius);
				}
				break;

			case 'X': // startup script to eXecute
				{
					String scriptName;
					in.readString(scriptName);
					factory->setScript(scriptName.get());
				}
				break;

			case 'P': // Physics
				{
					String physicsName;
					in.readString(physicsName);
					factory->setPhysics(physicsName.get());
				}
				break;

			case 'C': // Collide
				in.readString(collide);
				break;

			case 'I': // pIckable
				factory->setPickable(true);
				break;

			case 'O': // cOllideable
				factory->setCollideable(true);
				break;

			default:
				LogFatal("Unknown info code: " << code);
			}

		}

		factory->setCollide(collide.get());
		SimSchema::thingManager().addFactory(factory);
	}

}
