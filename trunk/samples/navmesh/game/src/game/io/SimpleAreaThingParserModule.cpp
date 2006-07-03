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


#include "SimpleAreaThingParserModule.hpp"
#include <cstring>

using namespace se_core;


namespace se_core {
	SimpleAreaThingParserModule
	::SimpleAreaThingParserModule(Parser& parser)
			: ParserModule(parser, ParserModule::GAME, ParserModule::AREA_THINGS, 1)  {
	}


	SimpleAreaThingParserModule
	::~SimpleAreaThingParserModule() {
	}


	void SimpleAreaThingParserModule
	::parse(InputStream& in) {
		Area* area = 0;

		int code = in.readInfoCode();
		Assert(code == 'N');
		{
			String tempString;
			in.readString(tempString);
			area = SimSchema::areaManager.area(tempString.get());
		}

		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'O': // object (thing or actor)
				readThing(in, *area);
				break;

			case 'C': // Cutscenes
				readMultiCutscene(in, area->multiSimObject(Area::MGOA_CUTSCENES));
				break;

			case 'E': // entrance
				short id = in.readShort();
				float x = in.readFloat();
				float y = in.readFloat();
				area->setEntrance(id, x, y);
				break;
			}
		}
		Assert(area);
		area->flip();
		LogMsg("Parsed things for: " << area->name());
	}


	void SimpleAreaThingParserModule
	::readThing(InputStream& in, Area& area) {
		WasHere();
		String tempString;
		WasHere();
		in.readString(tempString);
		WasHere();
		float x = in.readFloat();
		WasHere();
		float y = in.readFloat();
		WasHere();
		Coor c(CoorT::fromFloat(x), 0, CoorT::fromFloat(y));
		Quat4 q;
		WasHere();
		Thing* thing = area.spawn(tempString.get(), c, q);
		WasHere();

		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'R': // Radius
				{
					WasHere();
					float radius = in.readFloat();
					thing->nextPos().setRadius(CoorT::fromFloat(radius));
				}
				break;
			default:
				LogFatal("Illegal parameter to thing.");
			}
		}
		WasHere();
	}


	void SimpleAreaThingParserModule
	::readMultiCutscene(InputStream& in, MultiSimObject& mgo) {
		String* name;
		for(;;) {
			name = new String();
			in.readString(*name);
			if(strcmp("/", name->get()) == 0) {
				delete name;
				break;
			}
			if(!SimSchema::sortedSimObjectList().has(got_CUTSCENE, name->get())) {
				LogFatal("Tried to add non-existing cutscene.");
			}
			else {
				mgo.add(*SimSchema::sortedSimObjectList().cutscene(name->get()));
				delete name;
			}
		}
	}

}
