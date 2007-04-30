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


#include "CameraComponent.hpp"
#include "sim/SimEngine.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/Area.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/action/all.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/stat/Dictionary.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "../schema/ClientSchema.hpp"


using namespace se_core;

namespace se_client {
	CameraComponent
	::CameraComponent(Composite* owner)
			: Component(sct_CAMERA, owner), doesSee_(false) {
		posComponent_ = static_cast<PosComponent*>(owner_->component(sct_POS));
		Assert(posComponent_);
	}


	CameraComponent
	::~CameraComponent() {
	}

	void CameraComponent
	::setActive(bool state) {
	}


	void CameraComponent
	::zoneChanged(int zoneType, Composite* newZone, Composite* oldZone) {
		if(zoneType == st_AREA && posComponent_ == ClientSchema::camera) {
			if(newZone) {
				SimSchema::areaManager.setActive(Area::Ptr(newZone), 2);
				ZoneAreaComponent::Ptr oZone(oldZone);
				ZoneAreaComponent::Ptr nZone(newZone);
				if(oZone.isNull() || oZone->page().w_ != nZone->page().w_) {
					const char* newLevel = SimSchema::dictionary().name(DE_ZONE, nZone->page().w_);
					SimSchema::simEngine.setLevel(newLevel);
				}
			}
			else {
				SimSchema::areaManager.resetActive();
			}
		}

	}


	void CameraComponent
	::grabFocus() {
		if(posComponent_ == ClientSchema::camera)
			return;

		Composite* oldArea = 0;
		if(ClientSchema::camera) {
			if(ClientSchema::camera->pos().hasArea()) {
				oldArea = const_cast<Composite*>(ClientSchema::camera->pos().area()->owner());
			}
		}
		Composite* newArea = 0;
		if(posComponent_->pos().hasArea()) {
			newArea = const_cast<Composite*>(posComponent_->pos().area()->owner());
		}

		ClientSchema::camera = posComponent_;
		zoneChanged(st_AREA, newArea, oldArea);
	}

}
