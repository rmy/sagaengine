#include "BasicPre.hpp"
#include "SimpleActorParserModule.hpp"
#include "../thing/SimplePlayerFactory.hpp"
#include "../thing/SimpleCameraFactory.hpp"
#include "../thing/SimpleActorFactory.hpp"

using namespace se_core;

namespace se_basic {


	SimpleActorParserModule
	::SimpleActorParserModule(Parser& parser)
			: ParserModule(parser, ParserModule::GAME, ParserModule::THING, 2)  {
	}


	void SimpleActorParserModule
	::parse(InputStream& in) {

		SimpleActorFactory* factory;


		int code = in.readInfoCode();
		String* name = new String();
		in.readString(*name);

		switch(code) {
		case 'A':
			factory = new SimpleActorFactory(name);
			break;
		case 'C':
			factory = new SimpleCameraFactory(name);
			break;
		case 'P':
			factory = new SimplePlayerFactory(name);
			break;
		default:
			LogFatal("Unknown thing type '" << (char)(code) << "' in file " << in.name());
		}
		Assert(factory && "Forgot to create factory");
		String collide("none"); // Default collide

		enum { MAX_SPAWN_POINTS = 20 };
		int spawnPointCount = 0;
		ViewPoint* spawnPoints[ MAX_SPAWN_POINTS ];
		for(int i = 0; i < MAX_SPAWN_POINTS; ++i) {
			spawnPoints[i] = 0;
		}

		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'R': // Radius
				{
					float radius = in.readFloat();
					factory->setRadius(radius);
				}
				break;

			case 'X': // startup script to eXecute
				{
					String scriptName;
					in.readString(scriptName);
					factory->setScript(scriptName.get());
				}
				break;

			case 'P': // Physics
				{
					String physicsName;
					in.readString(physicsName);
					factory->setPhysics(physicsName.get());
				}
				break;

			case 'T': // Tag
				{
					int t = in.readDictionaryWord(DE_TAG);
					factory->setTag(t);
				}
				break;

			case 'D': // Default action
				{
					String actionName;
					in.readString(actionName);
					factory->setDefaultAction(actionName.get());
				}
				break;

			case 'C': // Collide
				in.readString(collide);
				break;

			case 'I': // pIckable
				factory->setPickable(true);
				break;

			case 'O': // cOllideable
				factory->setCollideable(true);
				break;

			case 'E': 
				{ // entrance
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
				LogFatal("Unknown code '" << (char)(code) << "' in file " << in.name());
			}

		}

		factory->setSpawnPoints(spawnPointCount, spawnPoints);
		factory->setCollide(collide.get());
		SimSchema::thingManager().addFactory(factory);
	}


	void SimpleActorParserModule
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
