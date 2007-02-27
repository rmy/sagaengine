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


#include "PlayerEncoderModule.hpp"
#include "io/parse/ParserModule.hpp"
#include "io/stream/OutputStream.hpp"
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

	PlayerEncoderModule
	::PlayerEncoderModule(Encoder& parser)
			: EncoderModule(parser, ParserModule::ENGINE, ParserModule::PLAYER, 1) {
	}


	PlayerEncoderModule
	::~PlayerEncoderModule() {
	}


	void PlayerEncoderModule
	::encode(OutputStream& out) {
		out.writeHeaderCode(headerCode());

		out.writeInfoCode('P');
		out.writeString(ClientSchema::player->owner()->name());
		out.writeInfoCode('E');
		out.writeString(ClientSchema::player->pos().area()->owner()->name());
		out.writeShort(0);
		out.writeInfoCode('G');
		out.writeInfoCode('/');

		out.writeInfoCode('C');
		out.writeString(ClientSchema::camera->owner()->name());
		out.writeInfoCode('E');
		out.writeString(ClientSchema::camera->pos().area()->owner()->name());
		out.writeShort(0);
		out.writeInfoCode('T');
		out.writeInfoCode('V');
		out.writeInfoCode('/');
	}

}

