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


#ifndef ZoneAreaComponentParser_hpp
#define ZoneAreaComponentParser_hpp

#include "io/parse/component/ComponentParser.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/sim.hpp"

namespace se_core {
	class _SeCoreExport ZoneAreaComponentParser : public ComponentParser {
	public:
		ZoneAreaComponentParser();
		virtual ~ZoneAreaComponentParser();
		ComponentFactory* parse(InputStream& in);

	protected:
		const DictionaryEntry dict_;
		enum { TYPE = sct_ZONE, SUBTYPE = 1 };
	};

	extern _SeCoreExport const ZoneAreaComponentParser parserZoneAreaComponent;
}

#endif
