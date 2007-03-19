#include "SimpleAreaThingParserModule.hpp"
#include "sim/spawn/SpawnAreaComponent.hpp"
#include <cstring>

using namespace se_core;


namespace se_basic {
	SimpleAreaThingParserModule
	::SimpleAreaThingParserModule(Parser& parser)
			: ParserModule(parser, ParserModule::GAME, ParserModule::AREA_THINGS, 1)  {
	}


	SimpleAreaThingParserModule
	::~SimpleAreaThingParserModule() {
	}


	void SimpleAreaThingParserModule
	::parse(InputStream& in) {
		const short MAX_AREAS = 1024;
		Area* areas[ MAX_AREAS];
		int areaCount = 0;

		short MAX_SPAWN_POINTS = 20;
		int spawnPointCount = 0;
		ViewPoint** spawnPoints = new ViewPoint*[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		int code;
		while((code = in.readInfoCode()) != 'Q' && !in.eof()) {
			switch(code) {
			case 'N':
				{
					String tempString;
					in.readString(tempString);
					if(SimSchema::areaManager.hasArea(tempString.get())) {
						areas[0] = SimSchema::areaManager.area(tempString.get());
						areaCount = 1;
					}
				}
				break;

			case 'F':
				{
					String tempString;
					in.readString(tempString);
					areaCount = SimSchema::areaManager.areasByFactory(tempString.get(), areas, MAX_AREAS);
					LogMsg(areaCount);
				}
				break;

			case 'A': // object (thing or actor)
				readThing(in, areaCount, areas);
				break;

			case 'C': // Cutscenes
				readMultiCutscene(in, areaCount, areas);
				break;

			case 'E': // entrance
				short id = in.readShort();
				Assert(id < MAX_SPAWN_POINTS);
				Assert(spawnPoints[id] == 0);

				ViewPoint* sp = new ViewPoint();
				readSpawnPoint(in, *sp);

				spawnPoints[id] = sp;
				if(id >= spawnPointCount) {
					spawnPointCount = id + 1;
				}
				break;
			}
		}
		for(int i = 0; i < areaCount; ++i) {
			if(spawnPointCount) {
				LogMsg("Set spawn points for: " << areas[i]->name());
				SpawnAreaComponent::Ptr aSpawn(*areas[i]);
				aSpawn->setSpawnPoints(spawnPointCount, spawnPoints);
			}
			//LogMsg("Flip children for: " << areas[i]->name());
			areas[i]->flipSpawns();
		}
		//LogMsg("Parsed things for: " << area->name());
	}


	void SimpleAreaThingParserModule
	::readThing(InputStream& in, int areaCount, se_core::Area** areas, SimComposite** parents) {
		String thingName;
		in.readString(thingName);

		ViewPoint vp;
		vp.setIdentity();

		bool isGrounded = false;
		bool isScaled = false;
		float radius = 1;

		int code;
		while((code = in.readInfoCode()) != '/') {
			// Start reading children
			if(code == '[') break;

			switch(code) {
			case 'G': // Grounded
				isGrounded = true;
				break;
			case 'S': // Scale
				{
					isScaled = true;
					radius = in.readFloat();
				}
				break;
			case 'T': // Transform
				{
					float x = in.readFloat();
					float y = in.readFloat();
					float z = in.readFloat();
					vp.coor_.set(CoorT::fromFloat(x), CoorT::fromFloat(y), CoorT::fromFloat(z));
				}
				break;
			case 'R': // Rotation
				{
					float yaw = in.readFloat();
					float pitch = in.readFloat();
					float roll = in.readFloat();
 					//LogMsg("R " << yaw << " " << pitch << " " << roll);
					vp.face_.setEuler(
									  BrayT::fromDeg(yaw)
									  , BrayT::fromDeg(pitch)
									  , BrayT::fromDeg(roll)
									  );
				}
				break;

			default:
				LogFatal("Illegal parameter to thing: " << (char)(code));
			}
		}

		SimComposite** siblings = new SimComposite*[areaCount];
		for(int i = 0; i < areaCount; ++i) {
			SimComposite* parent = (parents) ? parents[i] : 0;
			PosComponent* parentPos = PosComponent::get(parent);
			AssertFatal(areas[i]->terrainStyle(vp.coor_) != Pos::TS_VOID, thingName.get() << " - Area: " << areas[i]->name() << " - Factory: " << areas[i]->factory()->name() << " - Coor: " << vp.coor_);
			
			SimComposite* thing = areas[i]->spawn(thingName.get(), vp, 0, parentPos);
			PosComponent* p = PosComponent::get(thing);
			Assert(p);
			if(isScaled) {
				// Scale relative to the default raidus of the thing
				float r = CoorT::toFloat(p->nextPos().radius()) * radius;
				p->nextPos().setRadius(r);
			}
			p->nextPos().setGrounded(isGrounded);
			siblings[i] = thing;
		}

		if(code == '[') {
			//LogMsg('[');
			readChildren(in, areaCount, areas, siblings);

			// End of thing ('/') should follow
			code = in.readInfoCode();

			//LogMsg(']');
		}
		Assert(code == '/');
		delete[] siblings;
		//LogMsg(thing->name() << ": " << vp.toLog());
	}


	void SimpleAreaThingParserModule
	::readChildren(InputStream& in, int areaCount, Area** areas, SimComposite** parents) {
		int code;
		while((code = in.readInfoCode()) != ']') {
			//LogMsg("Code: " << (char)code);
			switch(code) {
			case 'A': // object (thing or actor)
				readThing(in, areaCount, areas, parents);
				break;
			}
		}
	}


	void SimpleAreaThingParserModule
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
				LogFatal("Illegal parameter to thing: " << (char)(code));
			}
		}
	}


	void SimpleAreaThingParserModule
	::readMultiCutscene(InputStream& in, int areaCount, Area** areas) {
		LogFatal("Not implemented");
		/*
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
				Cutscene& c = *SimSchema::sortedSimObjectList().cutscene(name->get());
				for(int i = 0; i < areaCount; ++i) {
					MultiSimObject& mgo = areas[i]->multiSimObject(Area::MGOA_CUTSCENES);
					mgo.add(c);
				}
				delete name;
			}
		}
		*/
	}

}
