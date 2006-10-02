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


#include "ClientEventBridge.hpp"
#include "ClientListeners.hpp"
#include "../schema/ClientSchema.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/SimListeners.hpp"
#include "sim/InitListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/stat/MultiSimObject.hpp"
#include "sim/stat/ReportingMultiSimObject.hpp"
#include "sim/SimObject.hpp"
#include "sim/SimEngine.hpp"
#include "sim/thing/Thing.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/thing/Camera.hpp"
#include "sim/thing/Player.hpp"
#include "sim/area/Area.hpp"
#include "sim/area/AreaManager.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace se_client {

	ClientEventBridge
	::ClientEventBridge() {
	}


	ClientEventBridge
	::~ClientEventBridge() {
	}


	void ClientEventBridge
	::simObjectAddedEvent(MultiSimObject& owner, SimObject& value) {
		Thing& thing = static_cast<Thing&>(value);
		if(value.id() != ClientSchema::camera->id()) {
			//ClientSchema::clientListeners.castThingEnteredCameraAreaEvent(thing);

			if(thing.pos().hasArea() && thing.pos().area()->isActive()) {
				ClientSchema::clientListeners.castThingSwitchedActiveAreaEvent(thing);
			}
			else {
				ClientSchema::clientListeners.castThingEnteredActiveZoneEvent(thing);
			}
		}
	}


	void ClientEventBridge
	::simObjectRemovedEvent(MultiSimObject& owner, SimObject& value) {
		/*
		Thing& thing = static_cast<Thing&>(value);
		if(value.id() != ClientSchema::camera->id()) {
			// If yes, cast thing left area event
			//ClientSchema::clientListeners.castThingLeftCameraAreaEvent(thing);

			if(!thing.nextPos().hasArea() || !thing.nextPos().area()->isActive()) {
				ClientSchema::clientListeners.castThingLeftActiveZoneEvent(thing);
			}
		}
		*/

	}


	void ClientEventBridge
	::cameraEnteredAreaEvent(Camera& caster, Area& area) {
		WasHere();
		SimSchema::areaManager.setActive(&area, 2);
		// Cast camera entered area event
		ClientSchema::clientListeners.castCameraEnteredAreaEvent(area);
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			LogMsg(SimSchema::areaManager.active(i)->name());
			SimSchema::areaManager.active(i)->reportingThings().setHandler(this);
		}
		WasHere();
	}


	void ClientEventBridge
	::cameraLeftAreaEvent(Camera& caster, Area& area) {
		for(int i = 0; i < SimSchema::areaManager.activeCount(); ++i) {
			SimSchema::areaManager.active(i)->reportingThings().setHandler(0);
		}
		// Cast camera left area event
		ClientSchema::clientListeners.castCameraLeftAreaEvent(area);
	}


	void ClientEventBridge
	::setCamera(Camera* newCamera) {
		WasHere();
		if(ClientSchema::camera == newCamera) return;
		Camera* oldCamera = ClientSchema::camera;
		ClientSchema::camera = newCamera;

		Area* oldArea = (oldCamera) ? const_cast<Area*>(oldCamera->pos().area()) : 0;
		Area* newArea = (newCamera) ? newCamera->nextPos().area() : 0;

		if(newArea != oldArea) {
			if(oldArea != 0) {
				cameraLeftAreaEvent(*oldCamera, *oldArea);
				//ClientSchema::clientListeners.castCameraLeftAreaEvent(*oldArea);
			}
			if(newArea != 0) {
				//SimSchema::areaManager.setActive(newArea);
				//ClientSchema::clientListeners.castCameraEnteredAreaEvent(*newArea);
				cameraEnteredAreaEvent(*newCamera, *newArea);
			}
		}
		if(oldCamera) {
			//if(oldArea) oldArea->reportingThings().setHandler(0);
			oldCamera->setCameraHandler(0);
		}
		if(newCamera) {
			//if(newArea) newArea->reportingThings().setHandler(this);
			newCamera->setCameraHandler(this);
		}
		WasHere();
	}


}
