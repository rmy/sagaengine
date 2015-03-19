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
#include "VorbisSound.hpp"
#include "PaStream.hpp"
#include "PaContext.hpp"
#include "PaManager.hpp"
#include "client/schema/ClientSchema.hpp"
#include "../lang/Sounds.hpp"
#include "../schema/PulseaudioSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/message/SoundCentral.hpp"
#include "sim/SimListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "sim/area/Area.hpp"
#include "io/schema/IoSchema.hpp"
#include "sim/pos/PosComponent.hpp"
#include "client/schema/ClientSchema.hpp"
#include <cstdio>

using namespace se_core;
using namespace se_client;



namespace se_pulseaudio {

	SoundPlayer
	::SoundPlayer() : system_(0), ambience_(0) {
		system_ = new PaManager();
	 
		AssertWarning(system_, "Couldn't create Pulseaudio system)");
		if(!system_)
		  return;

		system_->ambience()->setStreamListener(this);
		SimSchema::soundCentral.addListener(*this);
		SimSchema::engineListeners().addListener(*this);
	}


	SoundPlayer
	::~SoundPlayer() {
		system_->ambience()->setStreamListener(0);
		delete system_;
	}


	void SoundPlayer
	::init() {
	}


	void SoundPlayer
	::render() {
		for(int i = 0; system_ && i < 10; ++i) {
			system_->tick();
		}
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

		float volume = 0.0f;
		bool shouldLoop = false;
		VorbisSound* s = PulseaudioSchema::sounds.get(Sounds::MUSIC, snd, volume, shouldLoop);
		if(!s) {
			LogWarning("Couldn't play ambience: " << snd);
			ambience_ = 0;
			SimSchema::soundCentral.setAmbiencePlaying(false);
			return;
		}

		volume *= (SimSchema::soundCentral.musicVolume() / 100.0f);

		LogWarning("Playing ambience at volume: " << volume);
		ambience_ = system_->ambience()->play(*s, (int)(volume * 256),  shouldLoop);
		SimSchema::soundCentral.setAmbiencePlaying(true);
	}

	void SoundPlayer
	::streamEnded(PaStream* stream) {
		if(ambience_ == stream) {
			LogWarning("Ambience ended");
			SimSchema::soundCentral.setAmbiencePlaying(false);
			SimSchema::soundCentral.castMusicEnded();
			ambience_ = 0;
		}
	}


	void SoundPlayer
	::streamStopped(PaStream* stream) {
		if(ambience_ == stream) {
			LogWarning("Ambience stopped");
			SimSchema::soundCentral.setAmbiencePlaying(false);
			SimSchema::soundCentral.castMusicStopped();
			ambience_ = 0;
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
		float soundVolume;
		bool shouldLoop;
		VorbisSound* s = PulseaudioSchema::sounds.get(Sounds::SOUND, snd, soundVolume, shouldLoop);
		if(!s) {
			LogWarning("Couldn't play sound: " << snd);
			return;
		}
		float centralVolume = ((SimSchema::soundCentral.soundVolume()) / 100.0f);
		float playVolume = (soundVolume * centralVolume * d) * 0.84 + 0.16 * centralVolume ;
		LogDetail("Sound volume: " << centralVolume << " - V: " << soundVolume << " - PlayV: " << playVolume << " - Dist:  " << d);


		system_->fx()->play(*s, (int)(playVolume * 256),  shouldLoop);
	}


	VorbisSound* SoundPlayer
	::loadSound(const char* filename, bool shouldLoad) {
		char buffer[256];

		const char* dirname = IoSchema::dataPath;
		sprintf(buffer, "%s/snd/media/%s", dirname, filename);

		VorbisSound* snd = new VorbisSound(buffer);
		AssertWarning(snd, "Couldn't load sound: " << buffer);

		return snd;
	}

}
