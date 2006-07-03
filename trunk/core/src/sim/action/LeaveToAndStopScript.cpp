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


#include "LeaveToAndStopScript.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/Area.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/stat/SpawnPoint.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/thing/Thing.hpp"
#include "util/error/Log.hpp"



namespace se_core {


	void LeaveToAndStopScript
	::perform(long when, Actor& performer, Parameter& parameter) const {
		// Stop script
		performer.stopScript();

		//
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		LogMsg(p->areaName_);

		Area* area = SimSchema::areaManager.area(p->areaName_);
		SpawnPoint* sp = area->spawnPoint(p->entranceId_);
		performer.nextPos().setArea(*area, *sp);

	}


	void LeaveToAndStopScript
	::param(const char* areaName, short entranceId, Parameter& out) {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->areaName_ = areaName;
		p->entranceId_ = entranceId;
	}


	const LeaveToAndStopScript actionLeaveTo;
}
