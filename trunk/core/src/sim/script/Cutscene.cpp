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


#include "Cutscene.hpp"
#include "ShowingCutscene.hpp"
#include "../config/sim_config.hpp"
#include "../schema/SimSchema.hpp"
#include "../script/Script.hpp"
#include "../stat/SimObjectList.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../stat/MultiSimObjectIterator.hpp"
#include "../stat/SortedSimObjectList.hpp"
#include "../area/Area.hpp"
#include "../thing/Actor.hpp"
#include "util/type/TmpString.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {


	Cutscene
	::Cutscene(String* name)
		: SimObject(got_CUTSCENE, name->get()),
		  questGoal_(0), nameString_(name) {
		SimSchema::sortedSimObjectList().add(this);
	}


	Cutscene
	::~Cutscene() {
		SimSchema::sortedSimObjectList().remove(this);
		delete nameString_;
	}


	void Cutscene
	::setScripts(Area& area, Actor& performer, Actor* scriptTarget) {
		static ShowingCutscene showingCutscene;
		showingCutscene.freeMembers();
		showingCutscene.setCutscene(this);
		SimSchema::scriptTracker = 0;
		WasHere();
		SimSchema::didTrack = true;

		int scriptTarid = -1;
		if(scriptTarget) scriptTarid = scriptTarget->id();
		int performerId = performer.id();

		const Script* script ;
		TmpString tmp;
		sprintf(tmp.get(), "%s.performer", name());
		if(SimSchema::sortedSimObjectList().has(got_SCRIPT, tmp.get())) {
			WasHere();
			script = SimSchema::sortedSimObjectList().script(tmp.get());
			performer.setShowingCutscene(&showingCutscene, script);
		}
		else {
			performerId = -1; // Allow name used as script identifier
		}

		if(scriptTarget) {
			//scriptTarget->setTarget(&performer);
			sprintf(tmp.get(), "%s.target", name());
			if(SimSchema::sortedSimObjectList().has(got_SCRIPT, tmp.get())) {
				WasHere();
				script = SimSchema::sortedSimObjectList().script(tmp.get());
				scriptTarget->setShowingCutscene(&showingCutscene, script);
			}
			else {
				scriptTarid = -1; // Allow name used as script identifier
			}
		}

		SimObjectList::iterator_type it = area.multiSimObject(Area::MGOA_ACTORS).iterator();
		while(it != SimObjectList::NULL_NODE) {
			Actor* a = SimSchema::simObjectList.nextActor(it);
			if(a->id() == performerId || a->id() == scriptTarid)
				continue;

			if(a->cutscenes().contains(*this)
					|| a->cutsceneMemberships().contains(*this)
			   ) {
				sprintf(tmp.get(), "%s.%s", name(), a->name());
				script = SimSchema::sortedSimObjectList().script(tmp.get());
				a->setShowingCutscene(&showingCutscene, script);
			}
		}
	}

	/**
	* Used mainly for involving newcomers to the area into the cutscene.
	*/
	void Cutscene
	::setSingleScript(ShowingCutscene* showingCutscene, Actor& actor) {
		TmpString tmp;
		sprintf(tmp.get(), "%s.%s", name(), actor.name());
		if(SimSchema::sortedSimObjectList().has(got_SCRIPT, tmp.get())) {
			const Script* script = SimSchema::sortedSimObjectList().script(tmp.get());
			actor.setShowingCutscene(showingCutscene, script);
		}
	}

}
