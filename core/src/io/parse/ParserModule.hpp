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


#ifndef ParserModule_hpp
#define ParserModule_hpp

#include "io_parse.hpp"
#include "../stream/io_stream.hpp"

namespace se_core {
	class _SeCoreExport ParserModule {
	public:
		enum ModuleCode { 
			AREA = 'A'
			, AREA_THINGS = 'B'
			, RANDOM_AREA_THINGS = 'C'
			, CONFIG = 'F'
			, AREA_GRID = 'G'
			, HEIGHTMAP_AREA = 'H'
			, LANGUAGE = 'L'
			, MENU = 'M'
			, THING = 'O'
			, SPEECH_POSITION = 'P'
			, RANDOM_AREA = 'R'
			, CUTSCENE = 'S'
			, TERRAIN = 'T'
			, PLAYER = 'X'
			, PLUGIN = 'Z'
		};

		enum ModuleGroup { 
			ENGINE = 'E'
			, BASIC = 'B'
			, CLIENT = 'C'
			, SDL = 'S'
			, GBA = 'G'
			, OGRE = 'O'
			, LOGIC = 'L'
			, UI = 'U'
			, GAME = 'X'
			, FMOD = 'F' 
		};

		ParserModule(Parser &parser, unsigned char group, unsigned char code, int version);
		virtual ~ParserModule() {}
		int headerCode();
		const char* headerCodeString();
		virtual void parse(InputStream& in) = 0;

	private:
		int moduleGroup;
		int moduleCode;
		int moduleVersion;
	};

}

#endif
