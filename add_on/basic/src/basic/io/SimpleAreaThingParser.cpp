#include "SimpleAreaThingParser.hpp"
#include "sim/spawn/SpawnAreaComponent.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/area/Area.hpp"
#include "sim/zone/Exit.hpp"
#include "sim/custom/StatComponent.hpp"
#include "sim/signal/SignalComponent.hpp"
#include "sim/spawn/SpawnComponent.hpp"
#include "sim/spawn/SpawnComponentFactory.hpp"
#include "comp/CompositeFactory.hpp"
#include <cstring>

using namespace se_core;


namespace se_basic {
	SimpleAreaThingParser
	::SimpleAreaThingParser(ParseManager& parser)
			: Parser(parser, Parser::GAME, Parser::AREA_THINGS, 1)  {
	}


	SimpleAreaThingParser
	::~SimpleAreaThingParser() {
	}


	void SimpleAreaThingParser
	::parse(InputStream& in) {
		const short MAX_AREAS = 256;
		Area* areas[ MAX_AREAS];
		int areaCount = 0;

		short MAX_SPAWN_POINTS = 20;
		int spawnPointCount = 0;
		ViewPoint** spawnPoints = new ViewPoint*[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		static const int MAX_EXITS = 16;
		int exitCount = 0;
		Exit exits[MAX_EXITS];

		int code;
		while((code = in.readInfoCode()) != 'Q' && !in.eof()) {
			switch(code) {
			case 'D':
				{
					String tempString;
					in.readString(tempString);
					String name(in.oneDirAndBasename(), -in.basenameLen());
					name.append(tempString.get());
					if(SimSchema::areaManager.hasArea(name.get())) {
						areas[0] = SimSchema::areaManager.area(name.get());
						areaCount = 1;
					}
				}
				break;

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
				}
				break;

			case 'A': // object (thing or actor)
				readThing(in, areaCount, areas);
				break;

			case 'C': // Cutscenes
				readMultiCutscene(in, areaCount, areas);
				break;

			case 'E': { // entrance
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
			case 'X':
				{
					String area;
					in.readString(area);
					short entrance = in.readShort();
					exits[ exitCount ].area_.copy(area.get());
					exits[ exitCount ].entrance_ = entrance;
					++exitCount;
				}
				break;
			}
		}
		bool isSpawnPointsUsed = false;
		for(int i = 0; i < areaCount; ++i) {
			if(spawnPointCount) {
				//LogDetail("Set spawn points for: " << areas[i]->name() << " - " << in.name());
				SpawnAreaComponent::Ptr aSpawn(*areas[i]);
				isSpawnPointsUsed = true;
				aSpawn->setSpawnPoints(spawnPointCount, spawnPoints);
			}
			if(exitCount) {
				ZoneAreaComponent::Ptr aZone(*areas[i]);
				aZone->setExits(exits, exitCount);
			}
			//LogDetail("Flip children for: " << areas[i]->name());
			areas[i]->flipSpawns();
		}
		if(!isSpawnPointsUsed) {
			while(spawnPointCount > 0) delete spawnPoints[--spawnPointCount];
			delete[] spawnPoints;
		}
		//LogDetail("Parsed things for: " << area->name());
	}


	void SimpleAreaThingParser
	::readThing(InputStream& in, int areaCount, se_core::Area** areas, Composite** parents) {
		String thingName;
		in.readString(thingName);

		ViewPoint vp;
		vp.setIdentity();

		bool hasAnim[4];
		Anim anim[4];
		for(int i = 0; i < 4; ++i)
			hasAnim[i] = false;

		int isOn = -1;

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
 					//LogDetail("R " << yaw << " " << pitch << " " << roll);
					vp.face_.setEuler(
									  BrayT::fromDeg(yaw)
									  , BrayT::fromDeg(pitch)
									  , BrayT::fromDeg(roll)
									  );
				}
				break;

			case 'A': // Anim
				{
					int id = in.readShort();
					int movementMode = in.readShort();
					float startPos = in.readFloat();
					float pos = in.readFloat();
					float speed = in.readFloat();
					float weight = in.readFloat();

					hasAnim[id] = true;
					anim[id].setMovementMode(movementMode);
					anim[id].setStartPos(pos);
					anim[id].resetPos();
					anim[id].setStartPos(startPos);
					anim[id].setSpeed(speed);
					anim[id].setWeight(weight);
				}
				break;

			case 'I': // Signal
				{
					isOn = in.readShort();
					break;
				}

			default:
				LogFatal("Illegal parameter to thing: " << (char)(code));
			}
		}

		// Array to put created things. Is relevant if the
		// same thing is to be put in several areas. (Useful
		// when all areas of the same type has this thing.)
		Composite** siblings = new Composite*[areaCount];
		for(int i = 0; i < areaCount; ++i) {
			// Get parent area i (if parents array exists)
			Composite* parent = (parents) ? parents[i] : 0;
			// PosComponent of parent area
			PosComponent* parentPos = PosComponent::get(parent);
			AssertWarning(areas[i]->terrainStyle(vp.coor_) != Pos::TS_VOID, thingName.get() << " - Area: " << areas[i]->name() << " - Factory: " << areas[i]->owner()->factory()->name() << " - Coor: " << vp.coor_);
			// Is wanted position is at a spot where things may stand?
			if(areas[i]->terrainStyle(vp.coor_) == Pos::TS_VOID)
				continue;

			// Spawn the thing
			Composite* thing = areas[i]->spawn(thingName.get(), vp, 0, parentPos);
			// Place the thing
			PosComponent* p = PosComponent::get(thing);
			Assert(p);
			if(isScaled) {
				// Scale relative to the default raidus of the thing
				float r = CoorT::toFloat(p->nextPos().radius()) * radius;
				p->nextPos().setRadius(r);
			}
			// Clamp to ground??
			p->nextPos().setGrounded(isGrounded);
			// Init anims
			for(int j = 0; j < 4; ++j) {
				if(hasAnim[j]) {
					p->nextPos().anim(j).setAnim(anim[j]);
				}
			}
			// Mark as "should save" when saving to a savefile
			StatComponent::Ptr(*thing)->setShouldSave(true);

			// Send signal if active
			if(isOn >= 0) {
				SignalComponent::Ptr(*thing)->send(isOn == 0);
			}

			// Register sibling
			siblings[i] = thing;
		}

		if(code == '[') {
			//LogDetail('[');
			readChildren(in, areaCount, areas, siblings);

			// End of thing ('/') should follow
			code = in.readInfoCode();

			//LogDetail(']');
		}
		Assert(code == '/');
		delete[] siblings;
		//LogDetail(thing->name() << ": " << vp.toLog());
	}


	void SimpleAreaThingParser
	::readChildren(InputStream& in, int areaCount, Area** areas, Composite** parents) {
		int code;
		while((code = in.readInfoCode()) != ']') {
			//LogDetail("Code: " << (char)code);
			switch(code) {
			case 'A': // object (thing or actor)
				readThing(in, areaCount, areas, parents);
				break;
			}
		}
	}


	void SimpleAreaThingParser
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


	void SimpleAreaThingParser
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
