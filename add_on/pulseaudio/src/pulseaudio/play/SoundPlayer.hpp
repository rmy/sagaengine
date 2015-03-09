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


#ifndef SoundPlayer_hpp
#define SoundPlayer_hpp

#include "sim/message/SoundListener.hpp"
#include "sim/SimListener.hpp"
#include "PaStreamListener.hpp"

namespace se_pulseaudio {
	class SoundPlayer : public se_core::SoundListener, public PaStreamListener, public se_core::SimListener {
	public:
		SoundPlayer();
		~SoundPlayer();

		void ambienceEvent(const char* snd);
		void soundEvent(se_core::Actor& speaker, const char* snd);
		void render();

		// Callback when stream stopped
		void streamEnded(class PaStream* stream);
		void streamStopped(class PaStream* stream);

		class VorbisSound* loadSound(const char* filename, bool shouldLoad);
		void init();
		void cleanup();

		virtual void renderEvent(long when) { render(); }
		virtual void preSimTickEvent(long when) { render(); }
		virtual void postSimTickEvent(long when) { render(); }

	private:
		static const int MAX_CHANNELS = 32;
		class PaManager* system_;
		class PaStream* ambience_;

		//pulseaudio::AudioDevicePtr system_;
		//pulseaudio::OutputStreamPtr ambience_;
	};

}

#endif
