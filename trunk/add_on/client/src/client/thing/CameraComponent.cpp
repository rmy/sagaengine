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
#include "sim/thing/Actor.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "../schema/ClientSchema.hpp"


using namespace se_core;

namespace se_client {
	CameraComponent
	::CameraComponent(Composite* owner)
			: SimComponent(sct_CAMERA, owner) {
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
	::zoneChanged(int zoneType, Composite* newArea, SimComposite* oldArea) {
		if(zoneType == st_AREA && posComponent_ == ClientSchema::camera) {
			if(newArea) {
				SimSchema::areaManager.setActive(static_cast<Area*>(newArea), 2);
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

		SimComposite* oldArea = 0;
		if(ClientSchema::camera) {
			if(ClientSchema::camera->pos().hasArea()) {
				oldArea = const_cast<SimComposite*>(ClientSchema::camera->pos().area()->owner());
			}
		}
		SimComposite* newArea = 0;
		if(posComponent_->pos().hasArea()) {
			newArea = const_cast<SimComposite*>(posComponent_->pos().area()->owner());
		}

		ClientSchema::camera = posComponent_;
		zoneChanged(st_AREA, newArea, oldArea);
	}

}
