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


#include "Sounds.hpp"
#include "../play/SoundPlayer.hpp"
#include "../schema/FmodSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/Dictionary.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include <cstring>
#include <fmod.h>


using namespace se_core;

namespace se_fmod {
	Sounds
	::Sounds() : currentLanguage_(NORWEGIAN), soundCount_(0) {
		sounds_ = new Sound[ MAX_SOUNDS ];
	}

	Sounds
	::~Sounds() {
		FMOD_RESULT result;

		while(soundCount_ > 0) {
			--soundCount_;
			//TODO:
			//result = FMOD_Sound_Release(sounds_[ soundCount_ ].sound_);
			delete sounds_[ soundCount_ ].nameC_;
			delete sounds_[ soundCount_ ].soundC_;
			//if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
		}
		delete sounds_;
	}


	void Sounds
	::setLanguage(unsigned short language) {
		currentLanguage_ = language;
	}

	void Sounds
	::add(unsigned short language, SoundType type, String* nameC, String* soundC) {
		Assert(soundCount_ < MAX_SOUNDS);

		const char* name = nameC->get();
		FMOD_SOUND *sound = FmodSchema::soundPlayer->loadSound(soundC->get());
		if(!sound) {
			LogMsg("Couldn't load sound: " << soundC->get());
			return;
		}
		short index = find(type, name, language);
		if(index < soundCount_
		   && (sounds_[ index ].language_ != language
			   || sounds_[ index ].type_ != type || strcmp(sounds_[ index ].name_, name) != 0)
			) {
			for(int i = soundCount_; i > index; --i) {
				sounds_[ i ] = sounds_[ i - 1 ];
			}
		}
		sounds_[ index ].language_ = language;
		sounds_[ index ].type_ = type;
		sounds_[ index ].nameC_ = nameC;
		sounds_[ index ].soundC_ = soundC;
		sounds_[ index ].name_ = name;

		Assert(FmodSchema::soundPlayer && "Sound player object not created");
		sounds_[ index ].sound_ = sound;
		++soundCount_;
	}


	bool Sounds
	::isFound(short index, SoundType type, const char* name, unsigned short lang) {
		return (sounds_[ index ].language_ == lang && sounds_[ index ].type_ == type && strcmp(sounds_[index].name_, name) == 0);
	}


	short Sounds
	::find(SoundType type, const char* name, unsigned short lang) {
		short middle;
		short min = 0;
		short max = soundCount_;

		while(max > min) {
			middle = (max + min) / 2;
			Sound& s = sounds_[ middle ];
			if(s.language_ < lang
			   || (s.language_ == lang && s.type_ < type)
			   || (s.language_ == lang && s.type_ == type && strcmp(sounds_[middle].name_, name) < 0)) {
				min = middle + 1;
			}
			else {
				max = middle;
			}
		}

		// Must return nearest - used by add
		return min;
	}


	FMOD_SOUND* Sounds
	::get(SoundType type, const char* name) {
		// Is there a language specific sound?
		short index = find(type, name, currentLanguage_);
		if(!isFound(index, type, name, currentLanguage_)) {
			// Some sounds are not language specific
			index = find(type, name, ALL);
			if(!isFound(index, type, name, ALL)) {
				LogMsg("Searched for non-existing sound");
				return 0;
			}
		}
		LogMsg(sounds_[index].language_ << " " << type << " " << name);
		return sounds_[ index ].sound_;
	}


	unsigned short Sounds
	::languageId(const char* name) {
		return name[0] * 256 + name[1];
	}

}
