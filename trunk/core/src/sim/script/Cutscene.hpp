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


#ifndef Cutscene_hpp
#define Cutscene_hpp

#include "sim_script.hpp"
#include "../SimObject.hpp"
#include "../stat/sim_stat.hpp"
#include "../stat/MultiSimObject.hpp"
#include "../area/sim_area.hpp"
#include "../thing/sim_thing.hpp"
#include "../../util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport Cutscene : public SimObject {
	public:
		enum MultiSimObjectType {
			MGO_BEFORE = 0, MGO_AFTER, MGO_COUNT
		};

		Cutscene(String* name);
		virtual ~Cutscene();
		void setQuestGoal(QuestGoal* questGoal) { questGoal_ = questGoal; }
		QuestGoal* questGoal() { return questGoal_; }
		void setScripts(const Area& area, Actor& performer, Actor* scriptTarget = 0);
		void setSingleScript(ShowingCutscene* showingCutscene, Actor& actor);
		MultiSimObject& beforeTheseGoals() { return before_; }
		MultiSimObject& afterTheseGoals() { return after_; }
		const MultiSimObject& beforeTheseGoals() const { return before_; }
		const MultiSimObject& afterTheseGoals() const { return after_; }

	private:
		MultiSimObject before_;
		MultiSimObject after_;
		QuestGoal* questGoal_;
		String* nameString_; // For proper destruction of name strings
	};

}

#endif
