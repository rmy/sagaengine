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


#include "LevelDesignCleanRoom.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/stat/Dictionary.hpp"
#include "editor/comp/EditorAreaComponent.hpp"
#include <se_core.hpp>
#include <cstdio>

using namespace se_core;

namespace se_editor {

	void LevelDesignCleanRoom
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		PosComponent::Ptr pPos(perf);
		PosComponent::Ptr aPos(pPos->nextPos().area());
		se_editor::EditorAreaComponent::Ptr aEditor(pPos->nextPos().area());

		char oldname[256];
		char newname[256];
		sprintf(oldname, "%s/logic/area/skill_%d/thing/%s_things.txt", IoSchema::dataPath, SimSchema::skill, aPos->owner()->name());
		for(int i = 0; i < 256; ++i) {
			sprintf(newname, "%s/logic/area/skill_%d/thing/%s_things.txt.%d", IoSchema::dataPath, SimSchema::skill, aPos->owner()->name(), i);
			FILE* in = fopen(newname, "r");
			if(in) {
				fclose(in);
				continue;
			}
			break;
		}

		rename(oldname, newname);
		FILE* out = fopen(oldname, "wt");
		AssertFatal(out, oldname);
		fprintf(out, "XB01\nN %s\n", aPos->owner()->name());
		fclose(out);

		aEditor->startEditor();
	}


	short LevelDesignCleanRoom
	::duration(ActionComponent& performer, Parameter& parameter) const {
		return 1;
	}


	const LevelDesignCleanRoom actionLevelDesignCleanRoom;
}
