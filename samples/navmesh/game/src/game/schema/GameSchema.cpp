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


#include "GamePre.H"
#include "GameSchema.hpp"
#include "../io/all.hpp"
#include "../physics/all.hpp"

using namespace se_core;

namespace game {

	namespace GameSchema {
	}


	bool GameSchema::init() {
		// Create and register parser modules
		static NavMeshAreaParserModule nmAreaPM(IoSchema::parser());
		static SimpleAreaThingParserModule sAreaThingPM(IoSchema::parser());
		static SimpleActorParserModule sActorPM(IoSchema::parser());
		static SimpleCameraParserModule sCameraPM(IoSchema::parser());
		static SimplePlayerParserModule sPlayerPM(IoSchema::parser());

		// Create and register physics objects
		static const PhNavMesh phNavMesh;
		static const PhMissile phMissile;

		return true;
	}


	void GameSchema::cleanup() {
	}

}
