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


#include <cstdio>
#include "ParserModule.hpp"
#include "Parser.hpp"

namespace se_core {
	ParserModule::ParserModule(Parser &parser,
							   unsigned char group,
							   unsigned char code,
							   int version)
		: moduleGroup(group), moduleCode(code), moduleVersion(version)

	{
		parser.add(this);
	}


	int ParserModule::headerCode() {
		char buffer[5];
		sprintf(buffer, "%c%c%02d", moduleGroup, moduleCode, moduleVersion);
		int code = buffer[3] + buffer[2] * 256 + buffer[1] * 65536 + buffer[0] * 65536 * 256;
		return code;
	}

	const char* ParserModule::headerCodeString() {
		static char buffer[5];
		sprintf(buffer, "%c%c%02d", moduleGroup, moduleCode, moduleVersion);
		return buffer;
	}
}
