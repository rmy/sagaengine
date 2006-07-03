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


#include "AreaThingParserModule.hpp"
#include "../stream/InputStream.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/script/Cutscene.hpp"
#include "sim/stat/SpawnPoint.hpp"
#include "sim/stat/MultiSimObject.hpp"
#include "sim/stat/SortedSimObjectList.hpp"
#include "sim/area/Area.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/config/all.hpp"
#include "sim/thing/all.hpp"
#include "util/type/String.hpp"
#include "util/math/CoorT.hpp"
#include "util/error/Log.hpp"

#include <cstdio>
#include <cstring>



namespace se_core {
	AreaThingParserModule
	::AreaThingParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::ENGINE, ParserModule::AREA_THINGS, 1)  {
	}


	AreaThingParserModule
	::~AreaThingParserModule() {
	}


	void AreaThingParserModule
	::parse(InputStream& in) {
		Area* area = 0;
		short MAX_SPAWN_POINTS = 20;
		int spawnPointCount = 0;
		SpawnPoint** spawnPoints = new SpawnPoint*[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}
		String tempString;

		int code = ' ';
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'N': // name
				in.readString(tempString);
				area = SimSchema::areaManager.area(tempString.get());
				break;

			case 'O': // object (thing or actor)
				readThing(in, *area);
				break;

			case 'C': // Cutscenes
				readMultiCutscene(in, area->multiSimObject(Area::MGOA_CUTSCENES));
				break;

			case 'E': // entrance
				short id = in.readShort();
				coor_t x = CoorT::fromFloat(in.readFloat());
				coor_t z = CoorT::fromFloat(in.readFloat());
				Assert(id < MAX_SPAWN_POINTS);
				SpawnPoint* sp = new SpawnPoint();
				sp->displace_.set(x, 0, z);
				sp->face_.set(0, 0, 0, 0);
				spawnPoints[id] = sp;
				if(id > spawnPointCount) {
					spawnPointCount = id;
				}
				break;
			}
		}
		Assert(area);
		area->setSpawnPoints(spawnPointCount, spawnPoints);
		area->flipChildren();
		LogMsg("Parsed things for: " << area->name());
	}


	void AreaThingParserModule
	::readThing(InputStream& in, Area& area) {
		String tempString;
		in.readString(tempString);
		float x = in.readFloat();
		float y = in.readFloat();
		Coor c(CoorT::fromFloat(x), 0, CoorT::fromFloat(y));
		Thing* thing = area.spawn(tempString.get(), c, 0);

		int value, id;
		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'S': // single value
				id = in.readDictionaryWord(DE_SINGLE_VALUE);
				value = in.readInt();
				thing->setSingleValue(id, value);
				break;
			case 'F': // face direction
				value = in.readInt();
				//thing->nextPos().changeDirection(value * BRAY_RES / 2);
				thing->nextPos().setFaceDirection(value * BRAY_RES / 2);
				break;
			case 'A': // attribute value
				id = in.readDictionaryWord(DE_ATTRIBUTE);
				in.readString(thing->attribute(id));
				break;
			case 'R': // Radius
				{
					float radius = in.readFloat();
					thing->nextPos().setRadius(CoorT::fromFloat(radius));
				}
				break;
			default:
				LogFatal("Illegal parameter to thing.");
			}
		}
	}


	void AreaThingParserModule
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
