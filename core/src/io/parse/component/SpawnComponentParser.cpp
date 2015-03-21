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


#include "SpawnComponentParser.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/parse/ParseManager.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/spawn/SpawnComponentFactory.hpp"
#include "sim/script/Script.hpp"
#include "util/error/Log.hpp"
#include "util/vecmath/ViewPoint.hpp"



namespace se_core {
	const SpawnComponentParser parserSpawnComponent;

	SpawnComponentParser
	::SpawnComponentParser()
			: ComponentParser(IoSchema::parser(), TYPE, SUBTYPE)
			, dict_(DE_COMPONENT_TYPE, TYPE, "SPAWN") {
	}

	
	SpawnComponentParser
	::~SpawnComponentParser() {
	}


	ComponentFactory* SpawnComponentParser
	::parse(InputStream& in) {
		SpawnComponentFactory* factory = new SpawnComponentFactory();

		enum { MAX_SPAWN_POINTS = 20 };
		int spawnPointCount = 0;
		ViewPoint* spawnPoints[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			case 'E':
				{ // Spawn Point
					short id = in.readShort();
					Assert(id < MAX_SPAWN_POINTS);
					Assert(spawnPoints[id] == 0);

					ViewPoint* sp = new ViewPoint();
					readSpawnPoint(in, *sp);

					spawnPoints[id] = sp;
					if(id >= spawnPointCount) {
						spawnPointCount = id + 1;
					}
				}
				break;

			default:
				LogFatal("Unsupported code '" << (char)(code) << "' in file " << in.name());
			}
		}

		factory->setSpawnPoints(spawnPointCount, spawnPoints);
		return factory;
	}


	void SpawnComponentParser
	::readSpawnPoint(InputStream& in, ViewPoint& sp) {
		sp.coor_.reset();
		sp.face_.setIdentity();

		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'T': // Transform
				{
					float x = in.readFloat();
					float y = in.readFloat();
					float z = in.readFloat();
					sp.coor_.set(CoorT::fromFloat(x), CoorT::fromFloat(y), CoorT::fromFloat(z));
				}
				break;
			case 'R': // Transform
				{
					float yaw = in.readFloat();
					float pitch = in.readFloat();
					float roll = in.readFloat();
					sp.face_.setEuler(
									  BrayT::fromDeg(yaw)
									  , BrayT::fromDeg(pitch)
									  , BrayT::fromDeg(roll)
									  );
				}
				break;
			default:
				LogFatal("Unknown code '" << (char)(code) << "' in file " << in.name());
			}
		}
	}

}
