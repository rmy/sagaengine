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


#include "BasicPre.hpp"
#include "BasicSchema.hpp"
#include "../io/all.hpp"
#include "../physics/all.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"

using namespace se_core;

namespace se_basic {
	namespace BasicSchema {
		class _SeBasicExport AutoInit : public se_core::InitListener {
		public:
			AutoInit() {
				// Create and register parser modules
				static NavMeshAreaParser nAreaPM(IoSchema::parser());
				static SimpleAreaParser sAreaPM(IoSchema::parser());
				static SimpleAreaThingParser sAreaThingPM(IoSchema::parser());
				static SimpleActorParser sActorPM(IoSchema::parser());
				static WangAreaGridParser wangGridPM(IoSchema::parser());

				static SimpleAreaThingEncoderModule sAreaThingEM(IoSchema::encoder());

				// Register as init event listener
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Basic add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Basic add-on");
			}

			bool initEngineEvent() { return true; }
			void cleanupEngineEvent() {}

			bool initGameEvent() { return true;}
			void cleanupGameEvent() {}

			bool initLevelEvent() { return true; }
			void cleanupLevelEvent() {}

		} autoInit;
	}
}
