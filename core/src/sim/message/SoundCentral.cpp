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


#include "SoundCentral.hpp"
#include "SoundListener.hpp"
#include "Ambience.hpp"
#include "../thing/Actor.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {

	SoundCentral
	::SoundCentral() : listenerCount_(0), isAmbiencePlaying_(false), ambienceHandler_(0)
		, soundVolume_("Setting.SOUND_VOLUME", 8, 10), musicVolume_("Setting.MUSIC_VOLUME", 8, 10) {
	}

	SoundCentral
	::~SoundCentral() {
	}


	void SoundCentral
	::addListener(SoundListener &l) {
		AssertFatal(listenerCount_ < MAX_LISTENERS, "Added one listener too many");
		listeners[ listenerCount_++ ] = &l;
	}


	void SoundCentral
	::removeListener(SoundListener &l) {
		for(int i = 0; i < listenerCount_; ++i) {
			if(listeners[ i ] == &l) {
				listeners[ i ] = listeners[ --listenerCount_ ];
			}
		}
	}


	void SoundCentral
	::ambience(const char* snd) {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners[ i ]->ambienceEvent(snd);
		}
	}

	void SoundCentral
	::render() {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners[ i ]->render();
		}
	}


	void SoundCentral
	::setAmbienceHandler(const char* name) {
		setAmbienceHandler(*Ambience::lookup(name));
	}


	void SoundCentral
	::sound(Actor& speaker, const char* snd) {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners[ i ]->soundEvent(speaker, snd);
		}
	}

	void SoundCentral
	::castMusicStopped() {
		if(ambienceHandler_) {
			ambienceHandler_->musicStoppedEvent(*this);
		}
	}


	void SoundCentral
	::castMusicEnded() {
		if(ambienceHandler_) {
			ambienceHandler_->musicEndedEvent(*this);
		}
	}

}
