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


#include "PlayerParserModule.hpp"
#include "io/stream/InputStream.hpp"
#include "io/schema/IoSchema.hpp"
#include "client/event/ClientEventBridge.hpp"
#include "client/schema/ClientSchema.hpp"
#include "sim/sim.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/all.hpp"
#include "sim/area/all.hpp"
#include "sim/thing/all.hpp"
#include "util/error/Log.hpp"
#include "util/math/CoorT.hpp"
#include "util/type/String.hpp"
#include <cstdio>
#include <cstring>

namespace se_core {

	PlayerParserModule
	::PlayerParserModule(Parser& parser)
			: ParserModule(parser, ParserModule::ENGINE, ParserModule::PLAYER, 1) {
	}


	PlayerParserModule
	::~PlayerParserModule() {
	}


	void PlayerParserModule
	::parse(InputStream& in) {
		String tempString;

		int code = ' ';
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'P': // Player
				in.readString(tempString);
				LogMsg(tempString.get());
				ClientSchema::player = static_cast<Player*>(SimSchema::thingManager().create(tempString.get()));
				Assert(ClientSchema::player != 0);
				// TODO: ClientSchema::player->nextPos().changeMovement(0, 0);
				readInfo(in, ClientSchema::player);
				LogMsg("Pos - read:" << ClientSchema::player->nextPos().area()->name());
				ClientSchema::player->flip();
				LogMsg("Pos - flip");
				break;

			case 'C': // Camera
				// Init camera object
				in.readString(tempString);
				LogMsg(tempString.get());
				ClientSchema::floatingCamera = static_cast<Camera*>(SimSchema::thingManager().create(tempString.get()));
				Assert(ClientSchema::floatingCamera != 0);
				//TODO: ClientSchema::floatingCamera->nextPos().changeMovement(0, 0);
				SimSchema::simEngine.setSpecialMover(ClientSchema::floatingCamera);
				readInfo(in, ClientSchema::floatingCamera);
				ClientSchema::floatingCamera->flip();
				break;
			}
		}
	}


	void PlayerParserModule
	::readInfo(InputStream& in, Camera* camera) const {
		String tempString;
		Area* area;
		int value, id;
		String* stringValue;
		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'C': // Coor
				in.readString(tempString);
				LogMsg(tempString.get());
				area = SimSchema::areaManager.area(tempString.get());
				camera->nextPos().localCoor().x_ = CoorT::fromFloat(in.readFloat());
				camera->nextPos().localCoor().z_ = CoorT::fromFloat(in.readFloat());
				camera->nextPos().localFace().setIdentity();
				camera->nextPos().setArea(*area);
				break;

			case 'E': { // Entrance 
				in.readString(tempString);
				value = in.readShort();
				LogMsg(tempString.get());
				area = SimSchema::areaManager.area(tempString.get());
				Assert(area);
				const SpawnPoint* sp = area->spawnPoint(value);
				Assert(sp);
				camera->nextPos().setArea(*area, *sp);
				LogMsg("Pos - read:" << camera->nextPos().area()->name());
				}
				break;

			case 'S': // Statistic
				id = in.readDictionaryWord(DE_SINGLE_VALUE);
				value = in.readInt();
				camera->setSingleValue(id, value);
				break;

			case 'A': // Attribute
				id = in.readDictionaryWord(DE_ATTRIBUTE);
				stringValue = new String();
				//in.readString(*stringValue);
				//camera->attribute(id).set(stringValue);
				in.readString(camera->attribute(id));
				break;

			case 'V': // ViewPort
				ClientSchema::clientEventBridge.setCamera(camera);
				break;

			case 'T': // Camera tracking player
				Assert(camera != ClientSchema::player && "Player cannot track player");
				camera->nextPos().setParent(*ClientSchema::player);
				break;
			}
		}
	}

}

