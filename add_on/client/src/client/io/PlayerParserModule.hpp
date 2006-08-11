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


#ifndef client_io_PlayerParserModule_hpp
#define client_io_PlayerParserModule_hpp

#include "io/parse/ParserModule.hpp"
#include "io/stream/io_stream.hpp"
#include "sim/area/sim_area.hpp"
#include "sim/stat/sim_stat.hpp"
#include "sim/thing/sim_thing.hpp"


namespace se_core {
	class _SeClientExport PlayerParserModule : public ParserModule {
	public:
		PlayerParserModule(Parser& parser);
		virtual ~PlayerParserModule();
		void parse(InputStream& in);

	private:
		void readInfo(InputStream& in, Camera* camera) const;
	};

}

#endif