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


#include "ClientSchema.hpp"
#include "../event/ClientEventBridge.hpp"
#include "../event/ClientListeners.hpp"
#include "../io/all.hpp"
#include "../lang/Phrase.hpp"
#include "sim/InitListeners.hpp"
#include "sim/InitListener.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/thing/Player.hpp"
#include "sim/thing/Thing.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/system/util_system.hpp"


using namespace se_core;

namespace se_client {
	namespace ClientSchema {
		ClientEventBridge clientEventBridge; // OBJECT_IN_EWRAM;
		ClientListeners clientListeners; // OBJECT_IN_EWRAM;
		Phrase phrases OBJECT_IN_EWRAM;

		Player* player; // VAR_IN_EWRAM;
		Camera* floatingCamera; // VAR_IN_EWRAM;
		Camera* camera = 0; // VAR_IN_EWRAM;


		const struct _SeClientExport AutoInit : public se_core::InitListener {
			AutoInit() {
				// Register some file loaders
				static PlayerParserModule playerParserModule(se_core::IoSchema::parser());
				static LanguageParserModule languageParserModule(se_core::IoSchema::parser());

				// Client event bridge should listen to init events.
				SimSchema::initListeners().addListener(*this);
				LogMsg("Registered Client add-on");
			}


			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogMsg("Cleaned up Client add-on");
			}


			void initEngineEvent() {}
			void cleanupEngineEvent() {}
			void initGameEvent() {
				// Player and camera objects are initialised from data file.
			}

			void cleanupGameEvent() {
				if(ClientSchema::player) {
					ClientSchema::player->leaveCurrentArea();
					ClientSchema::player->reallyScheduleForDestruction();
				}

				if(ClientSchema::floatingCamera) {
					ClientSchema::floatingCamera->leaveCurrentArea();
					ClientSchema::floatingCamera->scheduleForDestruction();
				}
				ClientSchema::player = 0;
				ClientSchema::floatingCamera = 0;
				ClientSchema::camera = 0;
			}

		} autoInit;
	}
}
