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
#include "../schema/AudiereSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/message/SoundCentral.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/Area.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/pos/PosComponent.hpp"
#include "client/schema/ClientSchema.hpp"
#include <cstdio>

using namespace se_core;
using namespace se_client;
using namespace audiere;

namespace se_audiere {

	SoundPlayer
	::SoundPlayer() : system_(0), ambience_(0) {
		//enum { MEM_SIZE = 1024 * 1024 * 256 };
		//result = FMOD_Memory_Initialize(new unsigned char[ MEM_SIZE ], MEM_SIZE, 0, 0, 0);
		//if (result != FMOD_OK) LogWarning("FMOD error! (" << result << ") " << FMOD_ErrorString(result));

		system_ = audiere::OpenDevice();
		AssertFatal(system_, "Couldn't create Audiere system)");
		LogWarning(__FUNCTION__);

		// Avoid destruction from Audiere
		ref();
		system_->registerCallback(this);
		SimSchema::soundCentral.addListener(*this);
	}


	SoundPlayer
	::~SoundPlayer() {
		system_->clearCallbacks();
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
		/*
		for(int i = 0; i < MAX_CHANNELS; ++i) {
			system_->getChannel(i, &channel_);
			if(channel_) {
				channel_->stop();
			}
		}
		*/
	}

		
	void SoundPlayer
	::ambienceEvent(const char* snd) {
		if(!snd) {
			if(ambience_) {
				ambience_->stop();
				ambience_ = 0;
				SimSchema::soundCentral.setAmbiencePlaying(false);
			}
			return;
		}

		float volume;
		bool shouldLoop;
		audiere::OutputStreamPtr s = AudiereSchema::sounds.get(Sounds::MUSIC, snd, volume, shouldLoop);
		if(!s) {
			LogWarning("Couldn't play ambience: " << snd);
			ambience_ = 0;
			SimSchema::soundCentral.setAmbiencePlaying(false);
			return;
		}

		ambience_ = s;
		ambience_->setRepeat(shouldLoop);
		ambience_->setVolume(volume);
		ambience_->play();
		LogWarning("Playing ambience at volume: " << volume);
		SimSchema::soundCentral.setAmbiencePlaying(true);
	}

	void SoundPlayer
	::streamStopped(audiere::StopEvent* event) {
		if(ambience_.get() == event->getOutputStream()) {
			LogWarning("Ambience Stopped");
			bool isPlaying = ((ambience_ != 0) ? ambience_->isPlaying() : false);
			SimSchema::soundCentral.setAmbiencePlaying(isPlaying);
			if(event->getReason() == audiere::StopEvent::STREAM_ENDED) {
				SimSchema::soundCentral.castMusicEnded();
				ambience_ = 0;
			}
			else if(event->getReason() == audiere::StopEvent::STOP_CALLED) {
				SimSchema::soundCentral.castMusicStopped();
				ambience_ = 0;
			}
		}
		else {
			//LogWarning("Sound stopped");
		}
	}

	void SoundPlayer
	::soundEvent(Actor& sp, const char* snd) {
		//PosComponent& speaker = *(PosComponent::Ptr(sp));
		coor_t d = sp.nextPos().worldCoor().distance(se_client::ClientSchema::player->nextPos().worldCoor());
		if(d > 32)
			return;
		if(d < .5f) { 
			d = 1;
		}
		else {
			d = .5f / d;
		}

		LogDetail(sp.owner()->name() << " says " << snd << ": " << sp.nextPos().worldCoor());
		float volume;
		bool shouldLoop;
		audiere::OutputStreamPtr s = AudiereSchema::sounds.get(Sounds::SOUND, snd, volume, shouldLoop);
		if(!s) {
			LogWarning("Couldn't play sound: " << snd);
			return;
		}

		s->setRepeat(shouldLoop);
		s->setVolume(volume * d);
		s->play();
	}


	audiere::OutputStreamPtr SoundPlayer
	::loadSound(const char* filename, bool shouldLoad) {
		char buffer[256];

		// TODO: Load path from datapath.txt
		const char* dirname = IoSchema::dataPath;
		sprintf(buffer, "%s/fmod/media/%s", dirname, filename);

		audiere::OutputStreamPtr snd(OpenSound(system_, buffer, shouldLoad));
		AssertWarning(snd, "Couldn't load sound: " << buffer);

		return snd;
	}

}
