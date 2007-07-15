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


#ifndef CompositeParserModule_hpp
#define CompositeParserModule_hpp

#include "ParserModule.hpp"
#include "io/stream/io_stream.hpp"
#include "sim/script/sim_script.hpp"
#include "sim/stat/sim_stat.hpp"
#include "sim/thing/sim_thing.hpp"
#include "comp/list/comp_list.hpp"

namespace se_core {
	class _SeCoreExport CompositeParserModule : public ParserModule {
	public:
		/** Constructor */
		CompositeParserModule(ParseManager& parser);

		/** Parse a composite, calling component parsers as needed.
		 *
		 * File format:
		 *
		 * EO01
		 * L(evel)|A(rea)|T(hing)|E(existing thing) name
		 * 
		 * [Z component_type] (component parser formatted data)
		 * [Z component_type] (component parser formatted data)
		 * [...]
		 *
		 * [Q}
		 *
		 * component_type must be registered in the dictionary DE_COMPONENT_TYPE.
		 * A parser for the component type must be registered in IoSchema::parsers().
		 */
		void parse(InputStream& in);
	};

}

#endif
