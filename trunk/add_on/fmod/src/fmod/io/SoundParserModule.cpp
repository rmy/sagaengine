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


#include "SoundParserModule.hpp"
#include "../schema/FmodSchema.hpp"
#include "../lang/Sounds.hpp"
#include "io/stream/InputStream.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/stat/Dictionary.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "util/error/Log.hpp"
#include "sim/config/sim_config.hpp"
#include "util/type/String.hpp"
#include <cstdio>

using namespace se_core;

namespace se_fmod {

	SoundParserModule
	::SoundParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::FMOD, ParserModule::LANGUAGE, 1)  {
	}


	void SoundParserModule
	::parse(InputStream& in) {
		String* name;
		String* file;

		int code = 'X';
		while((code = in.readInfoCode()) != 'Q') {
			//printf("Code: %c\n", code);
			switch(code) {
			case 'L':
				{
					unsigned short language = in.readLanguageCode();
					unsigned short soundType = in.readDictionaryWord(DE_SOUND_TYPE);
					name = new String();
					in.readString(*name);
					LogMsg(name->get());

					float volume = in.readFloat();
					bool shouldLoop = (in.readInfoCode() == '+');
					file = new String();
					in.readString(*file);
					LogMsg(language << " " << soundType << ": " << name->get());
					FmodSchema::sounds.add(language, static_cast<Sounds::SoundType>(soundType), name, volume, file, shouldLoop);
					LogMsg(name->get());
				}
				break;
			default:
				LogFatal("Unkown info code :'" << (char)code << " in " << in.name());
			}
		}
	}
}
