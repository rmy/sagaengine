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


#include <se_core.hpp>
#include "SimpleAreaParserModule.hpp"
#include "../area/SimpleArea.hpp"
#include "../area/SimpleAreaFactory.hpp"
#include "sim/area/AreaManager.hpp"

using namespace se_core;

namespace se_basic {


	SimpleAreaParserModule
	::SimpleAreaParserModule(ParseManager& parser)
		: ParserModule(parser, ParserModule::GAME, ParserModule::AREA, 1)  {
	}


	void SimpleAreaParserModule
	::parse(InputStream& in) {
		String* name = new String();
		in.readString(*name);
		LogDetail(name->get());

		unsigned short w = in.readShort();
		unsigned short h = in.readShort();

		short doCreateInstance = false;
		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'C': // create instance
				doCreateInstance = true;
				break;
			default:
				LogFatal("Unknown code '" << (char)(code) << "' in file " << in.name());
				break;
			}
		}

		SimpleAreaFactory* f = new SimpleAreaFactory(name, w, h);
		SimSchema::areaManager.addFactory(f);
		if(doCreateInstance)
			SimSchema::areaManager.createArea(name->get(), name->get());
	}

}
