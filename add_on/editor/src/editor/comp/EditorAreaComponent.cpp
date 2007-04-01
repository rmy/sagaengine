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


#include "EditorManager.hpp"
#include "EditorAreaComponent.hpp"
#include "EditorComponent.hpp"
#include "sim/sim.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/spawn/SpawnAreaComponent.hpp"
#include "comp/list/NodeComponentList.hpp"
#include "util/error/Log.hpp"
#include "comp/CompositeFactory.hpp"
#include "io/stream/FileManager.hpp"
#include "io/schema/IoSchema.hpp"
#include <cstdio>

using namespace se_core;

namespace se_editor {

	EditorAreaComponent
	::EditorAreaComponent(Composite* owner, const se_core::ComponentFactory* factory) 
			: RootChildComponent(se_core::sct_EDITOR, owner, factory)
			, isEditing_(false) {
	}


	EditorAreaComponent
	::~EditorAreaComponent() {
	}


	void EditorAreaComponent
	::startEditor() {
		// Don't do this because of reverting, etc..
		//if(isEditing_)
		//	return;
		isEditing_ = true;

		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorComponent& c = static_cast<EditorComponent&>(it.next());
			if(c.owner()->component(sct_PLAYER) || c.owner()->component(sct_CAMERA)) {
				continue;
			}
			c.owner()->scheduleForDestruction();
		}
		char buffer[256];
		sprintf(buffer, "logic/area/thing/%s", owner()->name());
		IoSchema::fileManager->loadDirectory(buffer);

		for(int i = 0; i < 10; ++i) {
			entrances_[i].setIdentity();
			usedEntrances_[i] = false;
		}
		SpawnAreaComponent::Ptr aSpawn(*this);
		for(int i = 0; i < aSpawn->spawnPointCount(); ++i) {
			const ViewPoint* vp = aSpawn->spawnPoint(i);
			if(vp) {
				entrances_[i].setViewPoint(*vp);
				usedEntrances_[i] = true;
			}
		}
	}


	void EditorAreaComponent
	::exitEditor() {
		if(!isEditing_)
			return;
		isEditing_ = false;

		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorComponent& c = static_cast<EditorComponent&>(it.next());
			if(c.owner()->component(sct_PLAYER) || c.owner()->component(sct_CAMERA)) {
				continue;
			}
			c.owner()->scheduleForDestruction();
		}
		char buffer[256];
		sprintf(buffer, "logic/area/thing/%s", owner()->name());
		IoSchema::fileManager->loadDirectory(buffer);
	}


	void EditorAreaComponent
	::paste() {
		char name[256];
		sprintf(name, "clipboard/%s.txt", owner()->factory()->name());

		char filename[256];
		char buffer[256];
		sprintf(filename, "%s/logic/area/thing/%s_things.txt", IoSchema::dataPath, name);
		FILE* in = fopen(filename, "rt");
		if(in) {
			fgets(buffer, 256, in);
			fgets(buffer, 256, in);

			sprintf(filename, "%s/logic/area/thing/%s_things.txt", IoSchema::dataPath, owner()->name());
			FILE* out = fopen(filename, "wt");
			fprintf(out, "XB01\nN %s\n", owner()->name());

			while(!feof(in)) {
				fgets(buffer, 256, in);
				fputs(buffer, out);
			}

			fclose(out);
			fclose(in);
		}
	}



	void EditorAreaComponent
	::save() {
		char filename[256];
		sprintf(filename, "%s/logic/area/thing/%s_things.tmp.txt", IoSchema::dataPath, owner()->name());
		FILE* out = fopen(filename, "rt");
		if(out) {
			fclose(out);
			out = fopen(filename, "wt");
			fprintf(out, "XB01\nN %s\n", owner()->name());
			fclose(out);
		}

		save(owner()->name());
	}


	void EditorAreaComponent
	::copy() {
		char name[256];
		sprintf(name, "clipboard/%s.txt", owner()->factory()->name());
		save(name);
	}


	void EditorAreaComponent
	::save(const char* name) {
		char filename[256];
		sprintf(filename, "%s/logic/area/thing/%s_things.txt", IoSchema::dataPath, name);
		FILE* out = fopen(filename, "wt");
		fprintf(out, "XB01\nN %s\n", name);

		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorComponent& c = static_cast<EditorComponent&>(it.next());
			if(c.owner()->component(sct_PLAYER) || c.owner()->component(sct_CAMERA)) {
				continue;
			}
			Point3& p = c.start().coor_;
			Euler3& e = c.start().face_;
			fprintf(out, "A %s G T %03f %03f %03f R %f 0 0 /\n", c.owner()->name(), p.x_, p.y_, p.z_, BrayT::toDeg(e.yaw_));
		}
		fclose(out);

		sprintf(filename, "%s/logic/area/thing/%s_entrances.txt", IoSchema::dataPath, name);
		out = fopen(filename, "wt");
		fprintf(out, "XB01\nN %s\n", name);
		for(int i = 0; i < 10; ++i) {
			if(usedEntrances_[i]) {
				Point3& c = entrances_[i].coor_;
				Euler3& f = entrances_[i].face_;
				fprintf(out, "E %d T %f %f %f R %f %f %f /\n", i, c.x_, c.y_, c.z_,
					BrayT::toDeg(f.yaw_), BrayT::toDeg(f.pitch_), BrayT::toDeg(f.roll_)
					);
			}
		}
		fclose(out);
	}

	void EditorAreaComponent
	::setActive(bool state) {
		if(state) {
			NodeComponent* c = static_cast<NodeComponent*>(CompSchema::activeRoot().component(type_));
			if(c) {
				setParent(*c);
				if(EditorManager::singleton().isEditing()) {
					startEditor();
				}
				else {
					exitEditor();
				}
			}
		}
		else {
			resetParent();
		}
	}


	EditorComponent* EditorAreaComponent
	::findNearest(se_core::Point3& from) {
		EditorComponent* nearest = 0;
		float nearestDist = 0;

		NodeComponentList::Iterator it(children_);
		while(it.hasNext()) {
			EditorComponent& c = static_cast<EditorComponent&>(it.next());
			if(c.owner()->component(sct_PLAYER) || c.owner()->component(sct_CAMERA)) {
				continue;
			}
			Point3& p = c.start().coor_;
			float dist = p.distanceSquared(from);
			if(!nearest || dist < nearestDist) {
				nearestDist = dist;
				nearest = &c;
			}
		}
		return nearest;
	}

	void EditorAreaComponent
	::setEntrance(int id, se_core::ViewPoint& vp) {
		entrances_[ id ].setViewPoint(vp);
		usedEntrances_[id ] = true;
	}
}

