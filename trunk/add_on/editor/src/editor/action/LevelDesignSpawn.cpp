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


#include "LevelDesignSpawn.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/stat/Dictionary.hpp"
#include "editor/schema/EditorSchema.hpp"
#include "editor/comp/EditorComponent.hpp"
#include <se_core.hpp>
#include <cstdio>


using namespace se_core;

namespace se_editor {

	void LevelDesignSpawn
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		static const char* lastName;
		if(p->index_ == -1) {
			if(EditorSchema::lastSpawn) {
				PosComponent* lsPos = PosComponent::get(*EditorSchema::lastSpawn);
				SpawnComponent* aSpawn = SpawnComponent::get(*lsPos->nextPos().area());
				Point3& c = lsPos->nextPos().localCoor();
				Euler3& e = lsPos->nextPos().localFace();
				lsPos->nextPos().resetParent();
				PhysicsComponent::Ptr ph(*EditorSchema::lastSpawn);
				ph->popPhysics();

				char filename[256];
				sprintf(filename, "%s/logic/area/thing/%s_things.tmp.txt", IoSchema::dataPath, aSpawn->owner()->name());
				FILE* out = fopen(filename, "r+t");
				if(out) {
					fclose(out);
					out = fopen(filename, "at");
				} else {
					out = fopen(filename, "wt");
					fprintf(out, "XB01\nN %s\n", aSpawn->owner()->name());
				}
				fprintf(out, "A %s G T %f %f %f R %f 0 0 /\n", lastName, c.x_, c.y_, c.z_, BrayT::toDeg(e.yaw_));
				fclose(out);

				EditorComponent::Ptr lsEditor(*EditorSchema::lastSpawn);
				lsEditor->setStart(lsPos->nextPos());

				EditorSchema::lastSpawn = 0;
				return;
			}
		}

		if(EditorSchema::lastSpawn) {
			EditorSchema::lastSpawn->scheduleForDestruction();
			EditorSchema::lastSpawn = 0;
		}
		if(p->index_ < 0)
			return;

		int dictId = SimSchema::dictionary().id(DE_DICTIONARY_TYPE, "LEVEL_DESIGN_SPAWN");
		const char* name = 0;
		if(SimSchema::dictionary().hasId(dictId, p->index_)) {
			if(SimSchema::dictionary().hasId(dictId + 1, p->index_))
				name = SimSchema::dictionary().name(dictId + 1, p->index_);
			else
				name = SimSchema::dictionary().name(dictId, p->index_);
		}

		if(name) {
			SpawnComponent* pSpawn = SpawnComponent::get(perf);
			CollisionComponent::Ptr pCC(perf);

			Composite* spawn = pSpawn->spawn(name, 0);
			if(spawn) {
				EditorSchema::lastSpawn = spawn;
				lastName = SimSchema::dictionary().name(dictId, p->index_);
				ScriptComponent* s = ScriptComponent::get(*spawn);
				while(s->hasActiveScript()) {
					s->popScript();
				}
				PhysicsComponent* ph = PhysicsComponent::get(*spawn);
				if(SimSchema::dictionary().hasId(dictId + 2, p->index_)) {
					ph->pushPhysics(SimSchema::dictionary().name(dictId + 2, p->index_));
				}
				CollisionComponent::Ptr c(*spawn);
				if(c) {
					c->setIgnore(*pCC);
					c->setCollideable(false);
				}
				PosComponent* pPos = PosComponent::get(perf);
				PosComponent::Ptr sPos(*spawn);
				sPos->nextPos().setParent(*pPos);
				ph->pushPhysics("FollowMouse");
			}

			//pPos->nextPos().local_ = *aSpawn->spawnPoint(0);
			//pPos->nextPos().updateWorldViewPoint();
		}
	}


	short LevelDesignSpawn
	::duration(ActionComponent& performer, Parameter& parameter) const {
		return 1;
	}


	const LevelDesignSpawn& LevelDesignSpawn
	::param(int index, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->index_ = index;
		return *this;
	}



	const LevelDesignSpawn actionLevelDesignSpawn;
}
