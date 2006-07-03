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


#include "CutsceneParserModule.hpp"
#include "../stream/InputStream.hpp"
#include "../../sim/sim.hpp"
#include "../../sim/schema/SimSchema.hpp"
#include "../../sim/script/Cutscene.hpp"
#include "../../sim/script/TimelineScript.hpp"
#include "../../sim/script/QuestGoal.hpp"
#include "../../sim/stat/SortedSimObjectList.hpp"
#include "../../sim/stat/MultiSimObject.hpp"
#include "../../sim/area/Area.hpp"
#include "../../sim/area/AreaManager.hpp"
#include "../../sim/thing/ThingManager.hpp"
#include "../../util/type/String.hpp"
#include "../../util/error/Log.hpp"
#include <cstring>
#include <cstdio>


namespace se_core {


	CutsceneParserModule
	::CutsceneParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::ENGINE, ParserModule::CUTSCENE, 1)  {
	}


	void CutsceneParserModule
	::parse(InputStream& in) {
		String* name;
		Cutscene* cutscene = 0;

		int code = ' ';
		while((code = in.readInfoCode()) != 'Q') {
			//printf("Code: %c\n", code);
			switch(code) {
			case 'N': // Cutscene name
				name = new String();
				in.readString(*name);
				cutscene = new Cutscene(name);
				break;

			case 'O': // Actors owning (triggering) this cutscene
				readMultiOwner(in, cutscene);
				break;

			case 'M': // Actors that are members this cutscene
				readMultiMember(in, cutscene);
				break;

			case 'X': // Areas owning this cutscene
				readMultiArea(in, cutscene);
				break;

			case 'S': // Script
				readScript(in, cutscene->name(), 40);
				break;

			case 'Z': // Zero mode script
				readScript(in, cutscene->name(), 1);
				break;

			case 'A': // AFTER: QuestGoals that must be accomplished first
				readMultiQuest(in, cutscene->beforeTheseGoals());
				break;

			case 'B': // BEFORE: QuestGoals that must be accomplished after
				readMultiQuest(in, cutscene->afterTheseGoals());
				break;

			case 'R': // REWARD: QuestGoal that is rewarded
				name = new String();
				in.readString(*name);
				if(!SimSchema::sortedSimObjectList().has(got_QUEST_GOAL, name->get())) {
					cutscene->setQuestGoal(new QuestGoal(name));
				}
				else {
					cutscene->setQuestGoal(SimSchema::sortedSimObjectList().questGoal(name->get()));
					delete name;
				}
				break;

			default:
				LogFatal("Unsupported token");
				break;
			}
		}
	}


	void CutsceneParserModule
	::readScript(InputStream& in, const char* cutsceneName, short trackerPosCount) {
		String* name;
		String* attribute;
		name = new String();
		in.readString(*name);
		char* fullname = new char[ strlen(cutsceneName) + strlen(name->get()) + 2 ];
		sprintf(fullname, "%s.%s", cutsceneName, name->get());
		name->set(fullname);
		TimelineScript* script = new TimelineScript(name, trackerPosCount);
		int trackerPos = 0;
		int parameter = 0;
		name = new String();

		int code = 'X';
		while((code = in.readInfoCode()) != '/') {
			switch(code) {
			case 'C': // Command with no parameter
				trackerPos = in.readShort();
				in.readString(*name);
				script->addAction(trackerPos, name->get());
				break;

			case 'P': // Command with parameter
				trackerPos = in.readShort();
				in.readString(*name);
				parameter = in.readShort();
				script->addAction(trackerPos, name->get(), parameter);
				break;

			case 'A': // Command with attribute
				trackerPos = in.readShort();
				in.readString(*name);
				attribute = new String();
				in.readString(*attribute);
				script->addAction(trackerPos, name->get(), attribute);
				break;

			case 'B': // Command with attribute and parameter
				trackerPos = in.readShort();
				in.readString(*name);
				attribute = new String();
				in.readString(*attribute);
				parameter = in.readShort();
				script->addAction(trackerPos, name->get(), attribute, parameter);
				break;
			}
		}
		delete name;
	}


	void CutsceneParserModule
	::readMultiQuest(InputStream& in, MultiSimObject& mgo) {
		String* name;
		int count = 0;
		for(;;) {
			name = new String();
			in.readString(*name);
			if(strcmp("/", name->get()) == 0) {
				delete name;
				break;
			}
			if(!SimSchema::sortedSimObjectList().has(got_QUEST_GOAL, name->get())) {
				QuestGoal* g = new QuestGoal(name);
				mgo.add(*g);
			}
			else {
				mgo.add(*SimSchema::sortedSimObjectList().questGoal(name->get()));
				delete name;
			}
			++count;
			Assert(count > 10 && "More than 10 quest goals, loop hangs?");
		}
	}


	void CutsceneParserModule
	::readMultiOwner(InputStream& in, Cutscene* cutscene) {
		String* name;
		for(;;) {
			name = new String();
			in.readString(*name);
			if(strcmp("/", name->get()) == 0) {
				delete name;
				break;
			}
			//Assert(SimSchema::sortedSimObjectList().has(got_CUTSCENE, name->get()));
			//mgo.add(*SimSchema::sortedSimObjectList().get(got_CUTSCENE, name->get()));
			SimSchema::thingManager().addCutscene(name->get(), cutscene);
			delete name;
		}
	}

	void CutsceneParserModule
	::readMultiMember(InputStream& in, Cutscene* cutscene) {
		String* name;
		for(;;) {
			name = new String();
			in.readString(*name);
			if(strcmp("/", name->get()) == 0) {
				delete name;
				break;
			}
			SimSchema::thingManager().addCutsceneMembership(name->get(), cutscene);
			delete name;
		}
	}

	void CutsceneParserModule
	::readMultiArea(InputStream& in, Cutscene* cutscene) {
		String* name;
		for(;;) {
			name = new String();
			in.readString(*name);
			if(strcmp("/", name->get()) == 0) {
				delete name;
				break;
			}
			SimSchema::areaManager.area(name->get())->multiSimObject(Area::MGOA_CUTSCENES).add(*cutscene);
			delete name;
		}
	}

}
