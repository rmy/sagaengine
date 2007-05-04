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


#ifndef ComponentParser_hpp
#define ComponentParser_hpp

#include "io/parse/ComponentParserModule.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/sim.hpp"

namespace se_core {
	class _SeCoreExport ZoneAreaComponentParserModule : public ComponentParserModule {
	public:
		ZoneAreaComponentParserModule();
		virtual ~ZoneAreaComponentParserModule();
		ComponentFactory* parse(InputStream& in);

	protected:
		const DictionaryEntry dict_;
		enum { TYPE = sct_ZONE, SUBTYPE = 1 };
	};

	extern _SeCoreExport const ZoneAreaComponentParserModule parserZoneAreaComponent;
}

#endif
