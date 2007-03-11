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
#include "sim/area/Area.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/pos/PosComponent.hpp"
#include "client/schema/ClientSchema.hpp"
#include <fmod_errors.h>
#include <cstdio>

using namespace se_core;
using namespace se_client;

namespace se_fmod {
	void ERRCHECK(FMOD_RESULT result) {
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	SoundPlayer
	::SoundPlayer() : channel_(0), ambience_(0) {
		FMOD_RESULT result;

		//enum { MEM_SIZE = 1024 * 1024 * 256 };
		//result = FMOD_Memory_Initialize(new unsigned char[ MEM_SIZE ], MEM_SIZE, 0, 0, 0);
		//if (result != FMOD_OK) LogWarning("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		result = FMOD::System_Create(&system_);
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		result = system_->init(32, FMOD_INIT_NORMAL, NULL);
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		system_->setSoftwareChannels(128);

		SimSchema::soundCentral.addListener(*this);
	}


	SoundPlayer
	::~SoundPlayer() {
		FMOD_RESULT result;
		result = system_->release();
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	void SoundPlayer
	::init() {
	}


	void SoundPlayer
	::cleanup() {
		if(ambience_) {
			ambience_->stop();
			ambience_ = 0;
		}
		for(int i = 0; i < MAX_CHANNELS; ++i) {
			system_->getChannel(i, &channel_);
			if(channel_) {
				channel_->stop();
			}
		}
	}

		
	void SoundPlayer
	::ambienceEvent(char* snd) {
		LogWarning(snd);
		if(ambience_) {
			ambience_->stop();
			ambience_ = 0;
		}

		FMOD_RESULT result;
		float volume;
		FMOD::Sound *s = FmodSchema::sounds.get(Sounds::MUSIC, snd, volume);
		if(!s) {
			LogWarning("Couldn't play ambience: " << snd);
			return;
		}
		result = system_->playSound(FMOD_CHANNEL_FREE, s, 0, &ambience_);
		ambience_->setVolume(volume);
		ambience_->setPriority(0);
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	void SoundPlayer
	::soundEvent(Actor& sp, const char* snd) {
		PosComponent& speaker = *(PosComponent::Ptr(sp));
		FMOD_RESULT result;
		float volume;
		FMOD::Sound *s = FmodSchema::sounds.get(Sounds::SOUND, snd, volume);
		if(!s) {
			LogWarning("Couldn't play sound: " << snd);
			return;
		}

		result = system_->playSound(FMOD_CHANNEL_FREE, s, true, &channel_);
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		se_core::Point3 c(speaker.pos().worldCoor());
		c.sub(se_client::ClientSchema::player->pos().worldCoor());
		//c.scale(0.125);
		FMOD_VECTOR pos = { c.x_, c.y_, c.z_ };
		FMOD_VECTOR vel = { 0.0f,  0.0f, 0.0f };
		result = channel_->set3DAttributes(&pos, &vel);
		if (result != FMOD_OK) LogWarning("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		scale_t d = 1;
		int priority = 1;
		if(speaker.nextPos().area() != ClientSchema::camera->pos().area()) {
			d = .5f;
			priority = 2;
			
			if(speaker.nextPos().hasArea() && ClientSchema::camera->pos().hasArea()) {
				if(!speaker.nextPos().area()->toArea()->isNeighbour(*ClientSchema::camera->pos().area()->toArea())) {
					d = .25f;
					priority = 3;
				}
			}
			else {
				d = 0;
				priority = 255;
			}
		}
		channel_->setPriority(priority);
		/*

		coor_t d = speaker.pos().worldCoor().distance(se_client::ClientSchema::player->pos().worldCoor()) * .1f;
		if(d < .5f) { 
			d = 1;
		}
		else {
			d = .5f / d;
		}

		*/
		channel_->setVolume(volume * d);
		channel_->setLoopCount(0);


		result = channel_->setPaused(false);
		AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));
	}


	FMOD::Sound* SoundPlayer
	::loadSound(const char* filename, bool shouldLoop) {
		FMOD_RESULT result;
		FMOD::Sound* snd;
		char buffer[256];

		// TODO: Load path from datapath.txt
		const char* dirname = IoSchema::dataPath;
		sprintf(buffer, "%s/fmod/media/%s", dirname, filename);
		LogMsg(buffer);

		result = system_->createSound(buffer, FMOD_SOFTWARE | FMOD_3D | (shouldLoop ? 0 : FMOD_LOOP_NORMAL), 0, &snd);
		if (result != FMOD_OK) {
			AssertWarning(result == FMOD_OK, "FMOD error! (" << result << ") " << FMOD_ErrorString(result));
			snd = 0;
		}

		return snd;
	}

}
