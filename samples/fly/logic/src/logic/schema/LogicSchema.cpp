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


#include "LogicPre.hpp"
#include "LogicSchema.hpp"
#include "../physics/all.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"
#include "util/error/Log.hpp"

using namespace se_core;

namespace logic {

	namespace LogicSchema {
	}


	const struct _FlyLogicExport AutoInit : public se_core::InitListener {
		AutoInit() {
			SimSchema::initListeners().addListener(*this);
			LogDetail("Registered Game Logic");
		}


		~AutoInit() {
			LogDetail("Cleaned up Game Logic");
		}


		bool initEngineEvent() {
			// Create and register physics objects
			static const PhSimple phSimple;
			static const PhFly phFly;
			static const PhDanglingCamera phDanglingCamera;
			return true;
		}

		void cleanupEngineEvent() {}
		bool initGameEvent() {
			return true;
		}
		void cleanupGameEvent() {}
		bool initLevelEvent() {
			return true;
		}
		void cleanupLevelEvent() {}
	} autoInit;

}
