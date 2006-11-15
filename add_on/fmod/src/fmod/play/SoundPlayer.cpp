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


#include "SoundPlayer.hpp"
#include "client/schema/ClientSchema.hpp"
#include "../lang/Sounds.hpp"
#include "../schema/FmodSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/message/SoundCentral.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/pos/PosComponent.hpp"
#include <fmod_errors.h>
#include <cstdio>

using namespace se_core;

namespace se_fmod {
	void ERRCHECK(FMOD_RESULT result) {
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	SoundPlayer
	::SoundPlayer() : channel_(0) {
		FMOD_RESULT result;

		//enum { MEM_SIZE = 1024 * 1024 * 256 };
		//result = FMOD_Memory_Initialize(new unsigned char[ MEM_SIZE ], MEM_SIZE, 0, 0, 0);
		//if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		result = FMOD_System_Create(&system_);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		result = FMOD_System_Init(system_, 32, FMOD_INIT_NORMAL, NULL);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		SimSchema::soundCentral.addListener(*this);
	}


	SoundPlayer
	::~SoundPlayer() {
		FMOD_RESULT result;
		/*
		result = FMOD_Sound_Release(music_);
		ERRCHECK(result);
		result = FMOD_Sound_Release(sound_);
		ERRCHECK(result);
		*/

		result = FMOD_System_Close(system_);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	void SoundPlayer
	::ambienceEvent(char* snd) {
		FMOD_RESULT result;
		FMOD_SOUND *s = FmodSchema::sounds.get(Sounds::MUSIC, snd);
		if(!s) {
			LogMsg("Couldn't play ambience: " << snd);
			return;
		}
		result = FMOD_System_PlaySound(system_, FMOD_CHANNEL_FREE, s, 0, &channel_);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	void SoundPlayer
	::soundEvent(Actor& speaker, const char* snd) {
		FMOD_RESULT result;
		FMOD_SOUND *s = FmodSchema::sounds.get(Sounds::SOUND, snd);
		if(!s) {
			LogMsg("Couldn't play sound: " << snd);
			return;
		}

		result = FMOD_System_PlaySound(system_, FMOD_CHANNEL_FREE, s, true, &channel_);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		se_core::Point3 c(speaker.pos().worldCoor());
		c.sub(se_client::ClientSchema::player->pos().worldCoor());
		//c.scale(0.125);
        //FMOD_VECTOR pos = { c.x_, c.y_, c.z_ };
        //FMOD_VECTOR vel = { 0.0f,  0.0f, 0.0f };
		//result = FMOD_Channel_Set3DAttributes(channel_, &pos, &vel);
		//if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		coor_t d = speaker.pos().worldCoor().distance(se_client::ClientSchema::player->pos().worldCoor()) * .25;
		if(d < .5) { 
			d = 1;
		}
		else {
			d = .5 / d;
		}

		FMOD_Channel_SetVolume(channel_, d);


        result = FMOD_Channel_SetPaused(channel_, false);
		if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));


	}


	FMOD_SOUND* SoundPlayer
	::loadSound(const char* filename) {
		FMOD_RESULT result;
		FMOD_SOUND* snd;
		char buffer[256];

		// TODO: Load path from datapath.txt
		const char* dirname = IoSchema::dataPath;
		sprintf(buffer, "%s/fmod/media/%s", dirname, filename);
		LogMsg(buffer);
		//result = FMOD_System_CreateSound(system_, buffer, FMOD_SOFTWARE, 0, &snd);
		result = FMOD_System_CreateSound(system_, buffer, FMOD_HARDWARE | FMOD_3D, 0, &snd);
		if (result != FMOD_OK) {
			LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
			snd = 0;
		}

		return snd;
	}

}
