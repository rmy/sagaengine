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
#include "sim/thing/Player.hpp"
#include "sim/thing/Thing.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/system/util_system.hpp"


namespace se_fmod {
	namespace FmodSchema {
		Sounds sounds;
		SoundPlayer* soundPlayer = 0;

		bool init() {
			// Sound types
			static const DictionaryEntry stSound(DE_SOUND_TYPE, Sounds::SOUND, "SOUND");
			static const DictionaryEntry stMusic(DE_SOUND_TYPE, Sounds::MUSIC, "MUSIC");
			static const DictionaryEntry stSpeech(DE_SOUND_TYPE, Sounds::SPEECH, "SPEECH");


			// Register some file loaders
			static SoundParserModule soundParserModule(se_core::IoSchema::parser());

			soundPlayer = new SoundPlayer();
			LogMsg("Registered Fmod add-on");

			// return success
			return true;
		}

		void cleanup() {
			delete soundPlayer;
			LogMsg("Cleaned up Fmod add-on");
		}
	}
}
