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


#ifndef CutsceneParserModule_hpp
#define CutsceneParserModule_hpp

#include "ParserModule.hpp"
#include "../stream/io_stream.hpp"
#include "../../sim/script/sim_script.hpp"
#include "../../sim/stat/sim_stat.hpp"
#include "../../sim/thing/sim_thing.hpp"

namespace se_core {
	class CutsceneParserModule : public ParserModule {
	public:
		CutsceneParserModule(Parser& parser);
		void parse(InputStream& in);
		void readScript(InputStream& in, const char* cutsceneName, short trackerPosCount);
		void readMultiQuest(InputStream& in, MultiSimObject& mgo);
		void readMultiOwner(InputStream& in, Cutscene* cutscene);
		void readMultiMember(InputStream& in, Cutscene* cutscene);
		void readMultiArea(InputStream& in, Cutscene* cutscene);
	};

}

#endif
