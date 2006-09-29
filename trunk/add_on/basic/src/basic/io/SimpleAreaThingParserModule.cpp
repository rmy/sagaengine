#include "SimpleAreaThingParserModule.hpp"
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
		short MAX_AREAS = 1024;
		Area* areas[ MAX_AREAS];
		int areaCount = 0;

		short MAX_SPAWN_POINTS = 20;
		int spawnPointCount = 0;
		ViewPoint** spawnPoints = new ViewPoint*[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		int code = in.readInfoCode();
		switch(code) {
		case 'N':
			{
				String tempString;
				in.readString(tempString);
				areas[0] = SimSchema::areaManager.area(tempString.get());
				areaCount = 1;
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

		default:
			LogFatal("Unkown area type code: " << (char)(code));
		}

		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
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
		Assert(areaCount);
		for(int i = 0; i < areaCount; ++i) {
			if(spawnPointCount) {
				LogMsg("Set spawn points for: " << areas[i]->name());
				areas[i]->setSpawnPoints(spawnPointCount, spawnPoints);
			}
			areas[i]->flip();
		}
		//LogMsg("Parsed things for: " << area->name());
	}


	void SimpleAreaThingParserModule
	::readThing(InputStream& in, int areaCount, se_core::Area** areas, PosNode** parents) {
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

		PosNode* siblings[areaCount];
		for(int i = 0; i < areaCount; ++i) {
			LogMsg(areas[i]->name());
			PosNode* parent = (parents) ? parents[i] : 0;
			Thing* thing = areas[i]->spawn(thingName.get(), vp, 0, parent);
			if(isScaled) {
				// Scale relative to the default raidus of the thing
				float r = CoorT::toFloat(thing->nextPos().radius()) * radius;
				thing->nextPos().setRadius(r);
			}
			thing->nextPos().setGrounded(isGrounded);
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
		
		//LogMsg(thing->name() << ": " << vp.toLog());
	}


	void SimpleAreaThingParserModule
	::readChildren(InputStream& in, int areaCount, Area** areas, PosNode** parents) {
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
	}

}