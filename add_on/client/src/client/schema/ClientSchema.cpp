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
#include "../event/ClientListeners.hpp"
#include "../io/all.hpp"
#include "../lang/Phrase.hpp"
#include "sim/InitListeners.hpp"
#include "sim/InitListener.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/thing/Actor.hpp"
#include "client/thing/PlayerComponent.hpp"
#include "sim/pos/PosComponent.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/system/util_system.hpp"


using namespace se_core;

namespace se_client {
	namespace ClientSchema {
		ClientListeners clientListeners; // OBJECT_IN_EWRAM;
		Phrase* phrases_ = 0;
		Phrase& phrases() {
			Assert(phrases_);
			return *phrases_;
		}

		PlayerComponent* playerX; // VAR_IN_EWRAM;
		PosComponent* player; // VAR_IN_EWRAM;
		PosComponent* floatingCamera; // VAR_IN_EWRAM;
		PosComponent* camera = 0; // VAR_IN_EWRAM;


		const struct _SeClientExport AutoInit : public se_core::InitListener {
			AutoInit() {
				// Register some file loaders
				static PlayerParser playerParser(se_core::IoSchema::parser());
				static PlayerEncoderModule playerEncoderModule(se_core::IoSchema::encoder());
				static LanguageParser languageParser(se_core::IoSchema::parser());

				// Client event bridge should listen to init events.
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Client add-on");
			}


			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Client add-on");
			}


			bool initEngineEvent() {
				phrases_ = new Phrase;
				return true;
			}
			void cleanupEngineEvent() {
				delete phrases_;
			}
			bool initGameEvent() {
				// Player and camera objects are initialised from data file.
				return true;
			}
			void cleanupGameEvent() {
				ClientSchema::playerX = 0;
				ClientSchema::player = 0;
				ClientSchema::floatingCamera = 0;
				ClientSchema::camera = 0;
			}


			bool initLevelEvent() {
				return true;
			}

			void cleanupLevelEvent() {
			}

			bool startGameEvent() {
				return true;
			}

			void stopGameEvent() {
			}

		} autoInit;


		void touch() {
			se_core::SimSchema::touch();
			se_core::IoSchema::touch();
			autoInit;
		}
	}
}
