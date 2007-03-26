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


#include "ZoneAreaComponentParserModule.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/parse/Parser.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/config/sim_config.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/zone/ZoneAreaComponentFactory.hpp"
#include "sim/zone/Exit.hpp"



namespace se_core {
	const ZoneAreaComponentParserModule parserZoneAreaComponent;

	ZoneAreaComponentParserModule
	::ZoneAreaComponentParserModule()
			: ComponentParserModule(IoSchema::parser(), TYPE, SUBTYPE)
			, dict_(DE_COMPONENT_TYPE, TYPE, "ZONEAREA") {
	}

	
	ZoneAreaComponentParserModule
	::~ZoneAreaComponentParserModule() {
	}


	ComponentFactory* ZoneAreaComponentParserModule
	::parse(InputStream& in) {
		ZoneAreaComponentFactory* factory = new ZoneAreaComponentFactory();

		static const int MAX_EXITS = 16;
		int exitCount = 0;
		Exit exits[MAX_EXITS];

		int code = in.readInfoCode();
		Assert(code == '{');
		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			case 'X': {
					String* area = new String();
					in.readString(*area);
					short entrance = in.readShort();
					exits[ exitCount ].area_ = area;
					exits[ exitCount ].entrance_ = entrance;
					++exitCount;
				}
				break;
			default:
				LogFatal("Unsupported code '" << (char)(code) << "' in file " << in.name());
			}
		}
		factory->setExits(exits, exitCount);

		return factory;
	}

}
