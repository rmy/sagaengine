#include "BasicPre.hpp"
#include "SimpleActorParserModule.hpp"
#include "../thing/SimplePlayerFactory.hpp"
#include "../thing/SimpleCameraFactory.hpp"
#include "../thing/SimpleActorFactory.hpp"
//TODO!!!
//#include "physx/PhysXThingComponentFactory.hpp"

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
					BoundingBox b;
					b.setMin(-radius, 0, -radius);
					b.setMax(radius, 2 * radius, radius);
					factory->setBounds(b);
				}
				break;

			case 'S': // Stats
				{
					parseStats(in, factory);
					break;
				}
				break;

			case 'A': // Pos
				{
					parsePos(in, factory);
					break;
				}
				break;

			case 'G': // Stats
				{
					parseSignal(in, factory);
					break;
				}
				break;

			case 'B': // Bounds
				{
					BoundingBox b;
					b.minX_ = in.readFloat();
					b.minY_ = in.readFloat();
					b.minZ_ = in.readFloat();

					b.maxX_ = in.readFloat();
					b.maxY_ = in.readFloat();
					b.maxZ_ = in.readFloat();
					factory->setBounds(b);
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

			case 'I': // pIckable
				factory->setPickable(true);
				break;

			case 'C': // Collide
				in.readString(collide);
				break;

			case 'O': // cOllideable
				factory->setCollideable(true);
				break;

			case 'M': // cOllideable
				factory->setMass(in.readFloat());
				break;

			case 'F': // Friction
				{
					float linear = in.readFloat();
					float angular = in.readFloat();
					float bounceDecay = in.readFloat();
					factory->setFriction(linear, angular, bounceDecay);
				}
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

			case 'Z':
				{
					int type = in.readDictionaryWord(DE_COMPONENT_TYPE);
					SimComponentFactory* f = IoSchema::parser().parseComponent(in, type, 0);
					if(f) {
						factory->addComponent(f);
					}
				}
				break;

			default:
				LogFatal("Unknown code '" << (char)(code) << "' in file " << in.name());
			}

		}

		factory->setSpawnPoints(spawnPointCount, spawnPoints);
		if(!collide.equals("none")) {
			factory->setCollide(collide.get());
		}
		//factory->addComponent(new se_physx::PhysXThingComponentFactory());
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


	void SimpleActorParserModule
	::parseStats(InputStream& in, SimpleActorFactory* factory) {
		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'A':
				{
					int speed = in.readShort();
					int attack = in.readShort();
					int defense = in.readShort();
					int level = in.readShort();
					factory->setAbilities(speed, attack, defense, level);
				}
			}
		}
	}


	void SimpleActorParserModule
	::parseSignal(InputStream& in, SimpleActorFactory* factory) {
		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'S':
				{
					int id = in.readInt();
					factory->setSendSignal(id);
				}
				break;

			case 'R':
				{
					String signal;
					int mask = in.readInt();
					in.readString(signal);
					factory->setRecieveSignal(mask, signal.get());
				}
				break;
			}
		}
	}


	void SimpleActorParserModule
	::parsePos(InputStream& in, SimpleActorFactory* factory) {
		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'A':
				{
					int id = in.readShort();
					int animId = in.readDictionaryWord(DE_MOVEMENT_MODE);
					float pos = in.readFloat();
					float speed = in.readFloat();
					factory->setAnim(id, animId, pos, speed);
				}
				break;
			}
		}
	}


	SimpleComponentParserModule
	::SimpleComponentParserModule(const char* name, int type)
			: ComponentParserModule(IoSchema::parser(), type, 0), name_(DE_COMPONENT_TYPE, type, name, true) {
	}


	SimComponentFactory* StatComponentParserModule
	::parse(InputStream& in) {
		StatComponentFactory* factory = new StatComponentFactory();

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'A':
				{
					int speed = in.readShort();
					int attack = in.readShort();
					int defense = in.readShort();
					int level = in.readShort();
					factory->setAbilities(speed, attack, defense, level);
				}
				break;

			// Collectibles
			case 'C':
				{
					int c = in.readShort();
					factory->setCollectibles(c);
				}
			}
		}

		return factory;
	}


	SimComponentFactory* SignalComponentParserModule
	::parse(InputStream& in) {
		SignalComponentFactory* factory = new SignalComponentFactory();

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			// Abilites
			case 'S':
				{
					int id = in.readInt();
					factory->setSendSignal(id);
				}
				break;

			case 'R':
				{
					String signal;
					int mask = in.readInt();
					in.readString(signal);
					factory->setRecieveSignal(mask, signal.get());
				}
				break;
			}
		}

		return factory;
	}

}
