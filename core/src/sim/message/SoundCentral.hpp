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


#ifndef SoundCentral_hpp
#define SoundCentral_hpp

#include "sim_message.hpp"
#include "../thing/sim_thing.hpp"
#include "../setting/Setting.hpp"


namespace se_core {

	/** Sound message handling manager. */
	class _SeCoreExport SoundCentral {
	public:
		SoundCentral();
		virtual ~SoundCentral();

		void addListener(SoundListener& l);
		void removeListener(SoundListener& l);

		void sound(Actor& speaker, const char* snd);
		void ambience(const char* snd);
		void render();

		void castMusicStopped();
		void castMusicEnded();

		bool isAmbiencePlaying() { return isAmbiencePlaying_; }
		void setAmbiencePlaying(bool f) { isAmbiencePlaying_ = f; }
		void resetAmbienceHandler() {
			ambienceHandler_ = 0;
		}
		void setAmbienceHandler(const Ambience& handler) {
			ambienceHandler_ = &handler;
		}
		void setAmbienceHandler(const char* name);

		int soundVolume() const {
			return soundVolume_.value() * 10;
		}

		int musicVolume() const {
			return musicVolume_.value() * 10;
		}
	private:
		static const int MAX_LISTENERS = 10;
		SoundListener* listeners[ MAX_LISTENERS ];
		short listenerCount_;
		const Ambience* ambienceHandler_;
		bool isAmbiencePlaying_;
		Setting soundVolume_, musicVolume_;
	};

}

#endif
