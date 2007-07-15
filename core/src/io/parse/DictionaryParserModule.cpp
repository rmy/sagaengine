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


#include "DictionaryParserModule.hpp"
#include "../stream/InputStream.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/Dictionary.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/config/sim_config.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {


	DictionaryParserModule
	::DictionaryParserModule(ParseManager& parser)
		: ParserModule(parser, ParserModule::ENGINE, ParserModule::DICTIONARY, 1)  {
	}


	void DictionaryParserModule
	::parse(InputStream& in) {
		String name;
		// Dictionary name
		int dict = -1;
		int value = DE_FIRST_USER_TYPE;

		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'D':
				in.readString(name);
				dict = SimSchema::dictionary().id(DE_DICTIONARY_TYPE, name.get());
				value = 0;
				break;

			case 'A': // Append
				value = SimSchema::dictionary().highestId(dict) + 1;
				break;

			case 'V': // Value
				if(dict < 0) {
					LogFatal("You must fist specify a dictionary in file " << in.name());
				}
				value = in.readShort();
				break;

			case 'E': // Enum
				if(dict < 0) {
					LogFatal("You must fist specify a dictionary in file " << in.name());
				}
				in.readString(name);
				new DictionaryEntry(dict, value, name.get(), true);
				++value;
				break;

			default:
				LogFatal("Unsupported token " << (char)(code) << " in file " << in.name());
				break;
			}
		}
	}

}
