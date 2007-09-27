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


#include "CompositeParser.hpp"
#include "io/stream/InputStream.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/spawn/SpawnManager.hpp"
#include "comp/comp.hpp"
#include "comp/Composite.hpp"
#include "comp/CompositeFactory.hpp"
#include "comp/schema/CompSchema.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {


	CompositeParser
	::CompositeParser(ParseManager& parser)
		: Parser(parser, Parser::ENGINE, Parser::THING, 1)  {
	}


	void CompositeParser
	::parse(InputStream& in) {
		String* name = new String();
		CompositeFactory* factory;

		bool shouldAdd = true;
		int code = in.readInfoCode();
		in.readString(*name);

		switch(code) {
		case 'L': // Level
			factory = new CompositeFactory(st_LEVEL, name);
			break;
		case 'A': // Area
			factory = new CompositeFactory(st_AREA, name);
			break;
		case 'T': // Thing/Actor
			factory = new CompositeFactory(st_ACTOR, name);
			break;
		case 'E': // Existing entity
			factory = SpawnManager::singleton().factory(name->get());
			shouldAdd = false;
			break;
		default:
			LogFatal("Unknown thing type '" << (char)(code) << "' in file " << in.name());
		}
		AssertFatal(factory, "Forgot to create factory in file " << in.name());

		while(!in.eof() && (code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'Z':
				{
					int type = in.readDictionaryWord(DE_COMPONENT_TYPE);
					ComponentFactory* f = IoSchema::parser().parseComponent(in, type, 0);
					if(f) {
						factory->addComponent(f);
					}
				}
				break;

			default:
				LogFatal("Unknown code '" << (char)(code) << "' in file " << in.name());
			}
		}

		if(shouldAdd) {
			SimSchema::spawnManager().addFactory(factory);
		}
	}

}
