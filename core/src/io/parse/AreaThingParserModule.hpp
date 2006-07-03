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


#ifndef AreaThingParserModule_hpp
#define AreaThingParserModule_hpp

#include "../stream/io_stream.hpp"
#include "../../sim/area/sim_area.hpp"
#include "../../sim/stat/sim_stat.hpp"
#include "ParserModule.hpp"

namespace se_core {
	class AreaThingParserModule  : public ParserModule {
	public:
		AreaThingParserModule(Parser& parser);
		virtual ~AreaThingParserModule();
		void parse(InputStream& in);

	private:
		void readThing(InputStream& in, Area& area);
		void readMultiCutscene(InputStream& in, MultiSimObject& mgo);
	};

}

#endif
