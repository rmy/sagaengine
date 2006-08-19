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

using namespace se_core;

namespace se_basic {


	SimpleAreaParserModule
	::SimpleAreaParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::GAME, ParserModule::AREA, 1)  {
	}


	void SimpleAreaParserModule
	::parse(InputStream& in) {
		String* name = new String();
		in.readString(*name);
		LogMsg(name->get());

		unsigned short w = in.readShort();
		unsigned short h = in.readShort();

		SimpleAreaFactory* f = new SimpleAreaFactory(name, w, h);
		SimSchema::areaManager.addFactory(f);
		//SimSchema::areaManager.createArea(name->get(), name->get());
	}

}
