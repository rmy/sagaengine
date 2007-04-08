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


#include "FmodSchema.hpp"
#include "../io/all.hpp"
#include "../play/all.hpp"
#include "sim/stat/Dictionary.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "fmod/lang/Sounds.hpp"
#include "sim/InitListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/pos/PosComponent.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/system/util_system.hpp"

using namespace se_core;

namespace se_fmod {
	namespace FmodSchema {
		Sounds sounds;
		SoundPlayer* soundPlayer = 0;

		struct AutoInit : public se_core::InitListener {
			AutoInit() {

				// Sound types
				static const se_core::DictionaryEntry stSound(se_core::DE_SOUND_TYPE, Sounds::SOUND, "SOUND");
				static const se_core::DictionaryEntry stMusic(se_core::DE_SOUND_TYPE, Sounds::MUSIC, "MUSIC");
				static const se_core::DictionaryEntry stSpeech(se_core::DE_SOUND_TYPE, Sounds::SPEECH, "SPEECH");


				// Register some file loaders
				static SoundParserModule soundParserModule(se_core::IoSchema::parser());
				
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Fmod add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Fmod add-on");
			}


			bool initEngineEvent() {
				soundPlayer = new SoundPlayer();
				return true;
			}

			void cleanupEngineEvent() {
				delete soundPlayer;
			}

			bool initGameEvent() {
				return true;
			}

			void cleanupGameEvent() {
			}

			bool initLevelEvent() {
				soundPlayer->init();
				return true;
			}

			void cleanupLevelEvent() {
				soundPlayer->cleanup();
			}
		} autoInit;


		void touch() {
			se_core::SimSchema::touch();
			se_core::IoSchema::touch();
			autoInit;
		}
	}
}
