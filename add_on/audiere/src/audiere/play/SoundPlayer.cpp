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
		system_ = audiere::OpenDevice();
		AssertWarning(system_, "Couldn't create Audiere system)");
		if(!system_)
		  return;

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

		volume *= (SimSchema::soundCentral.musicVolume() / 100.0f);

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
		if(d > 40)
			return;
		if(d < 1) { 
			d = 1;
		}
		else {
			d = .3f + .7f / d;
		}

		LogDetail(sp.owner()->name() << " says " << snd << ": " << sp.nextPos().worldCoor());
		float volume;
		bool shouldLoop;
		audiere::OutputStreamPtr s = AudiereSchema::sounds.get(Sounds::SOUND, snd, volume, shouldLoop);
		if(!s) {
			LogWarning("Couldn't play sound: " << snd);
			return;
		}
		volume *= (SimSchema::soundCentral.soundVolume() / 100.0f);
		LogDetail("Sound volume: " << SimSchema::soundCentral.soundVolume() << " - " << d);

		s->setRepeat(shouldLoop);
		s->setVolume(volume * d);
		s->play();
	}


	audiere::OutputStreamPtr SoundPlayer
	::loadSound(const char* filename, bool shouldLoad) {
		char buffer[256];

		const char* dirname = IoSchema::dataPath;
		sprintf(buffer, "%s/snd/media/%s", dirname, filename);

		//audiere::OutputStreamPtr snd(OpenSound(system_, buffer, shouldLoad));
		audiere::OutputStreamPtr snd(OpenSound(system_, buffer, shouldLoad, FF_OGG));
		AssertWarning(snd, "Couldn't load sound: " << buffer);

		return snd;
	}

}
