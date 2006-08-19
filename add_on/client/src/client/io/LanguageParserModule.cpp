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


#include "LanguageParserModule.hpp"
#include "../lang/Phrase.hpp"
#include "../schema/ClientSchema.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"
#include "io/stream/InputStream.hpp"
#include "io/schema/IoSchema.hpp"
#include <cstdio>

using namespace se_core;

namespace se_client {

	LanguageParserModule
	::LanguageParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::CLIENT, ParserModule::LANGUAGE, 1)  {
	}


	void LanguageParserModule
	::parse(InputStream& in) {
		String* name;
		String* label;

		int code = 'X';
		while((code = in.readInfoCode()) != 'Q') {
			//printf("Code: %c\n", code);
			switch(code) {
			case 'L':
				{
					unsigned short language = in.readLanguageCode();
					unsigned short phraseType = in.readPhraseType();
					name = new String();
					in.readString(*name);
					LogMsg(name->get());
					label = new String();
					in.readString(*label);
					ClientSchema::phrases.addPhrase(language, static_cast<Phrase::PhraseType>(phraseType), name, label);
					LogMsg(name->get());
				}
				break;
			default:
				LogFatal((sprintf(log_msg(), "Unkown info code %c in \"%s\"", code, in.name()), log_msg()));
			}
		}
	}

}
