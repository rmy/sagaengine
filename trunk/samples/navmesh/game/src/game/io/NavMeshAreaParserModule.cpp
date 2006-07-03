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


#include <se_core.hpp>
#include "NavMeshAreaParserModule.hpp"
#include "../area/NavMeshArea.hpp"

using namespace se_core;

namespace game {

	NavMeshAreaParserModule
	::NavMeshAreaParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::GAME, ParserModule::AREA, 1)  {
	}


	void NavMeshAreaParserModule
	::parse(InputStream& in) {
		String* name = new String();
		in.readString(*name);
		LogMsg(name->get());

		unsigned short w = in.readShort();
		unsigned short h = in.readShort();
		LogMsg(w << ", " << h);

		unsigned short dataSize = in.readShort();
		ByteArray* data = new ByteArray();
		in.readByteArray(*data, dataSize);

		NavMeshArea* area = new NavMeshArea(name, w, h, data);
		SimSchema::areaManager.addArea(area);
	}

}
