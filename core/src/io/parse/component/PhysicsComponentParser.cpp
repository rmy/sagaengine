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


#include "PhysicsComponentParser.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/parse/ParseManager.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/physics/PhysicsComponentFactory.hpp"
#include "sim/script/Script.hpp"



namespace se_core {
	const PhysicsComponentParser parserPhysicsComponent;

	PhysicsComponentParser
	::PhysicsComponentParser()
			: ComponentParser(IoSchema::parser(), TYPE, SUBTYPE)
			, dict_(DE_COMPONENT_TYPE, TYPE, "PHYSICS") {
	}

	
	PhysicsComponentParser
	::~PhysicsComponentParser() {
	}


	ComponentFactory* PhysicsComponentParser
	::parse(InputStream& in) {
		PhysicsComponentFactory* factory = new PhysicsComponentFactory();

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			case 'F': // Friction
				{
					float linear = in.readFloat();
					float angular = in.readFloat();
					float bounceDecay = in.readFloat();
					factory->setFriction(linear, angular, bounceDecay);
				}
				break;

			case 'M': // Mass
				factory->setMass(in.readFloat());
				break;

			case 'P': // Physics
				{
					String physicsName;
					in.readString(physicsName);
					factory->setPhysics(physicsName.get());
				}
				break;

			default:
				LogFatal("Unsupported code '" << (char)(code) << "' in file " << in.name());
			}
		}

		return factory;
	}

}
