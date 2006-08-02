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


namespace se_core {
	Sounds
	::Sounds() : currentLanguage_(NORWEGIAN), soundCount_(0) {
	}

	Sounds
	::~Sounds() {
		FMOD_RESULT result;

		while(soundCount_ > 0) {
			--soundCount_;
			delete namesC_[ soundCount_ ];
			delete soundsC_[ soundCount_ ];
			result = FMOD_Sound_Release(sounds_[ soundCount_ ]);
			//if (result != FMOD_OK) LogMsg("FMOD error! (" << result << ") " << FMOD_ErrorString(result));
		}
	}


	void Sounds
	::setLanguage(unsigned short language) {
		currentLanguage_ = language;
	}

	void Sounds
	::add(unsigned short language, SoundType type, String* nameC, String* soundC) {
		Assert(soundCount_ < MAX_SOUNDS);

		const char* name = nameC->get();
		const char* sound = soundC->get();
		short index = find(type, name, language);
		if(index < soundCount_
		   && (languages_[ index ] != language
			   || types_[ index ] != type || strcmp(names_[ index ], name) != 0)
			) {
			for(int i = soundCount_; i > index; --i) {
				languages_[ i ] = languages_[ i - 1 ];
				types_[ i ] = types_[ i - 1 ];
				names_[ i ] = names_[ i - 1 ];
				sounds_[ i ] = sounds_[ i - 1 ];

				namesC_[ i ] = namesC_[ i - 1 ];
				soundsC_[ i ] = soundsC_[ i - 1 ];
			}
		}
		languages_[ index ] = language;
		types_[ index ] = type;
		namesC_[ index ] = nameC;
		soundsC_[ index ] = soundC;
		names_[ index ] = name;

		LogMsg(languages_[index] << " " << type << " " << name);
		LogMsg(index << ": " << names_[index]);
		//sounds_[ index ] = sound;
		Assert(FmodSchema::soundPlayer && "Sound player object not created");
		sounds_[ index ] = FmodSchema::soundPlayer->loadSound(sound);
		++soundCount_;
	}

	bool Sounds
	::isFound(short index, SoundType type, const char* name, unsigned short lang) {
		return (languages_[ index ] == lang && types_[ index ] == type && strcmp(names_[index], name) == 0);
	}


	short Sounds
	::find(SoundType type, const char* name, unsigned short lang) {
		short middle;
		short min = 0;
		short max = soundCount_;

		while(max > min) {
			middle = (max + min) / 2;
			if(languages_[ middle ] < lang
			   || (languages_[ middle ] == lang && types_[ middle ] < type)
			   || (languages_[ middle ] == lang && types_[ middle ] == type && strcmp(names_[middle], name) < 0)) {
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
		short index = find(type, name, currentLanguage_);
		if(!isFound(index, type, name, currentLanguage_)) {
			index = find(type, name, ALL);
			Assert(isFound(index, type, name, ALL) && "Searched for non-existing sound");
			//return 0;
		}
		LogMsg(languages_[index] << " " << type << " " << name);
		Assert((languages_[index] == ALL || languages_[index] == currentLanguage_));
		Assert(types_[ index ] == type);
		Assert(strcmp(names_[index], name) == 0);
		return sounds_[ index ];
	}


	unsigned short Sounds
	::languageId(const char* name) {
		return name[0] * 256 + name[1];
	}

}
