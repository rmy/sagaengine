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
#include "sim/action/ActionComponent.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/spawn/SpawnAreaComponent.hpp"
#include "util/error/Log.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/ViewPoint.hpp"


using namespace se_core;


namespace se_basic {


	void LeaveToAndStopScript
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Actor& performer = *perf.toActor();
		// Stop script
		performer.stopScript();

		//
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		LogMsg(p->areaName_);

		Area* area = SimSchema::areaManager.area(p->areaName_);
		const ViewPoint* sp = SpawnAreaComponent::Ptr(*area)->spawnPoint(p->entranceId_);
		performer.nextPos().setArea(*PosComponent::get(*area), *sp);

	}


	void LeaveToAndStopScript
	::param(const char* areaName, short entranceId, Parameter& out) {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		p->areaName_ = areaName;
		p->entranceId_ = entranceId;
	}


	const LeaveToAndStopScript actionLeaveTo;
}
