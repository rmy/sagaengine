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


#ifndef PluginParserModule_hpp
#define PluginParserModule_hpp

#include "io/stream/io_stream.hpp"
#include "sim/area/sim_area.hpp"
#include "sim/stat/sim_stat.hpp"
#include "io/parse/ParserModule.hpp"

namespace se_pc {
	class _SePcExport PluginParserModule  : public se_core::ParserModule {
	public:
		PluginParserModule(se_core::Parser& parser);
		virtual ~PluginParserModule();
		void parse(se_core::InputStream& in);
	};
}

#endif
