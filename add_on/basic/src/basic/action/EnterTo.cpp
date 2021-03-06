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


#include "EnterTo.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/Area.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/thing/Actor.hpp"
#include "util/error/Log.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "sim/spawn/SpawnAreaComponent.hpp"


using namespace se_core;

namespace se_basic {

	void EnterTo
	::perform(long when, ActionComponent& perf, Parameter& parameter) const {
		Actor& performer = *Actor::Ptr(perf);;
		Param* p = static_cast<Param*>(parameter.data(sizeof(Param)));
		const char* name = p->areaName_;
		short id = p->id_;
		LogDetail(name);

		performer.nextMove().resetSpeed();
		Area* area = SimSchema::areaManager.area(name);
		Assert(area);
		
		const ViewPoint* sp = SpawnAreaComponent::Ptr(*area)->spawnPoint(id);
		performer.nextPos().local_.setViewPoint( *sp );
		if(!performer.pos().hasArea(*area)) {
			performer.nextPos().setArea(*PosComponent::Ptr(area));
		}
	}


	void EnterTo
	::param(const char* areaName, short entranceId, Parameter& out) const {
		Param* p = static_cast<Param*>(out.data(sizeof(Param)));
		*p = Param(areaName, entranceId);
	}

	const EnterTo actionEnterTo;
}
