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

		short MAX_SPAWN_POINTS = 20;
		int spawnPointCount = 0;
		SpawnPoint** spawnPoints = new SpawnPoint*[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		int code = in.readInfoCode();
		Assert(code == 'N');
		{
			String tempString;
			in.readString(tempString);
			area = SimSchema::areaManager.area(tempString.get());
		}

		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'A': // object (thing or actor)
				readThing(in, *area);
				break;

			case 'C': // Cutscenes
				readMultiCutscene(in, area->multiSimObject(Area::MGOA_CUTSCENES));
				break;

			case 'E': // entrance
				short id = in.readShort();
				Assert(id < MAX_SPAWN_POINTS);
				Assert(spawnPoints[id] == 0);

				SpawnPoint* sp = new SpawnPoint();
				readSpawnPoint(in, *sp);

				spawnPoints[id] = sp;
				if(id >= spawnPointCount) {
					spawnPointCount = id + 1;
				}
				break;
			}
		}
		Assert(area);
		area->setSpawnPoints(spawnPointCount, spawnPoints);
		area->flip();
		LogMsg("Parsed things for: " << area->name());
	}


	void SimpleAreaThingParserModule
	::readThing(InputStream& in, se_core::Area& area, PosNode* parent) {
		String thingName;
		in.readString(thingName);

		ViewPoint vp;
		vp.setIdentity();

		bool isGrounded = false;
		bool isScaled = false;
		float radius = 1;

		int code;
		while((code = in.readInfoCode()) != '/') {
			LogMsg("Code: " << (char)code);
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
 					LogMsg("S " << radius);
				}
				break;
			case 'T': // Transform
				{
					float x = in.readFloat();
					float y = in.readFloat();
					float z = in.readFloat();
					LogMsg("T " << x << " " << y << " " << z);
					vp.coor_.set(CoorT::fromFloat(x), CoorT::fromFloat(y), CoorT::fromFloat(z));
				}
				break;
			case 'R': // Transform
				{
					float yaw = in.readFloat();
					float pitch = in.readFloat();
					float roll = in.readFloat();
 					LogMsg("R " << yaw << " " << pitch << " " << roll);
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

		Thing* thing = area.spawn(thingName.get(), vp, 0, parent);
		if(isScaled) {
			// Scale relative to the default raidus of the thing
			float r = CoorT::toFloat(thing->nextPos().radius()) * radius;
			thing->nextPos().setRadius(r);
		}
		thing->nextPos().setGrounded(isGrounded);

		if(code == '[') {
			LogMsg('[');
			readChildren(in, area, *thing);

			// End of thing ('/') should follow
			code = in.readInfoCode();

			LogMsg(']');
		}
		Assert(code == '/');
		
		LogMsg(thing->name() << ": " << vp.toLog());
	}


	void SimpleAreaThingParserModule
	::readChildren(InputStream& in, Area& area, PosNode& parent) {
		int code;
		while((code = in.readInfoCode()) != ']') {
			LogMsg("Code: " << (char)code);
			switch(code) {
			case 'A': // object (thing or actor)
				readThing(in, area, &parent);
				break;
			}
		}
	}


	void SimpleAreaThingParserModule
	::readSpawnPoint(InputStream& in, SpawnPoint& sp) {
		sp.displace_.reset();
		sp.face_.setIdentity();

		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'T': // Transform
				{
					float x = in.readFloat();
					float y = in.readFloat();
					float z = in.readFloat();
					sp.displace_.set(CoorT::fromFloat(x), CoorT::fromFloat(y), CoorT::fromFloat(z));
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
