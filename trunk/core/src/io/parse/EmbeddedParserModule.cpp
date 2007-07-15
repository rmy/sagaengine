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


#include "EmbeddedParserModule.hpp"
#include "../stream/InputStream.hpp"
#include "../schema/IoSchema.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/config/sim_config.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {


	EmbeddedParserModule
	::EmbeddedParserModule(ParseManager& parser)
		: ParserModule(parser, ParserModule::ENGINE, ParserModule::EMBEDDED, 1)  {
	}


	void EmbeddedParserModule
	::parse(InputStream& in) {
		while(!in.eof()) {
			IoSchema::parser().parse(in);
		}
	}

}
