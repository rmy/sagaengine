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
#include "client/schema/ClientSchema.hpp"
#include "client/thing/all.hpp"
#include "sim/sim.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/all.hpp"
#include "sim/area/all.hpp"
#include "sim/thing/all.hpp"
#include "client/thing/all.hpp"
#include "sim/pos/all.hpp"
#include "util/error/Log.hpp"
#include "util/math/CoorT.hpp"
#include "util/type/String.hpp"
#include <cstdio>
#include <cstring>

using namespace se_core;

namespace se_client {

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
		while((code = in.readInfoCode()) != 'Q' && !in.eof()) {
			switch(code) {
			case 'P': 
				{ // Player
					in.readString(tempString);
					LogMsg(tempString.get());
					SimComposite* a = SimSchema::thingManager().create(tempString.get());
					Assert(a);
					PosComponent* pos = static_cast<PosComponent*>(a->component(sct_POS));
					PhysicsComponent* physics = PhysicsComponent::get(*a);
					Assert(pos);
					CameraComponent* camera = new CameraComponent(a);
					ClientSchema::playerX = new PlayerComponent(a);
					Assert(ClientSchema::playerX != 0);
					ClientSchema::player = pos;
					Assert(ClientSchema::player != 0);
					readInfo(in, pos, camera);
					pos->flip();
					physics->nextMove().work_.vp_.face_.yaw_ = pos->nextPos().localFace().yaw_;
				}
				break;

			case 'C': // Camera
				{
					// Init camera object
					in.readString(tempString);
					SimComposite* a = SimSchema::thingManager().create(tempString.get());
					Assert(a);
					PosComponent* pos = static_cast<PosComponent*>(a->component(sct_POS));
					Assert(pos);
					CameraComponent* camera = new CameraComponent(a);
					ClientSchema::floatingCamera = pos;
					Assert(ClientSchema::floatingCamera != 0);
					readInfo(in, pos, camera);
					pos->flip();
				}
				break;
			}
		}
	}


	void PlayerParserModule
	::readInfo(InputStream& in, PosComponent* pos, CameraComponent* camera) const {

		String tempString;
		Area* area;
		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'C': // Coor
				in.readString(tempString);
				LogMsg(tempString.get());
				area = SimSchema::areaManager.area(tempString.get());
				Assert(area);
				pos->nextPos().localCoor().x_ = CoorT::fromFloat(in.readFloat());
				pos->nextPos().localCoor().z_ = CoorT::fromFloat(in.readFloat());
				pos->nextPos().localFace().setIdentity();
				pos->nextPos().setArea(*PosComponent::get(*area));
				break;

			case 'E': 
				{ // Entrance 
					in.readString(tempString);
					int value = in.readShort();
					LogMsg(tempString.get());
					area = SimSchema::areaManager.area(tempString.get());
					Assert(area);
					const ViewPoint* sp = area->spawnPoint(value);
					Assert(sp);
					pos->nextPos().setArea(*PosComponent::get(*area), *sp);
				}
				break;

			case 'A': 
				{ // Area
					in.readString(tempString);
					int c = in.readInfoCode();
					Assert(c == 'T');

					ViewPoint sp;
					sp.coor_.x_ = in.readFloat();
					sp.coor_.y_ = in.readFloat();
					sp.coor_.z_ = in.readFloat();

					c = in.readInfoCode();
					Assert(c == 'R');
					sp.face_.yaw_ = BrayT::fromDeg(in.readFloat());
					sp.face_.pitch_ = BrayT::fromDeg(in.readFloat());
					sp.face_.roll_ = BrayT::fromDeg(in.readFloat());

					area = SimSchema::areaManager.area(tempString.get());
					pos->nextPos().setArea(*PosComponent::get(*area), sp);

					c = in.readInfoCode();
					Assert(c == '/');
				}
				break;

			case 'V': // ViewPort
				camera->grabFocus();
				break;

			case 'T': // Camera tracking player
				Assert(pos != ClientSchema::player && "Player cannot track player");
				pos->nextPos().setParent(*ClientSchema::player);
				break;
			}
		}
	}

}

