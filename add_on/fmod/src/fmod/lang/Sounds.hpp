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


#ifndef Sounds_hpp
#define Sounds_hpp

#include "util/type/util_type.hpp"
#include <fmod.hpp>

namespace se_fmod {
	class Sounds {
	public:
		Sounds();
		virtual ~Sounds();
		enum SoundType { UNDEFINED, SOUND, MUSIC, SPEECH, TYPE_COUNT };
		static const unsigned short ALL = '-' * 256 + '-';
		static const unsigned short ENGLISH = 'U' * 256 + 'K';
		static const unsigned short NORWEGIAN = 'N' * 256 + 'O';
		static const unsigned short FRENCH = 'F' * 256 + 'R';

		void add(unsigned short language, SoundType type, se_core::String* name, float volume, se_core::String* filename, bool shouldLoop);
		short find(SoundType type, const char* name, unsigned short lang);
		FMOD::Sound* get(SoundType type, const char* name, float& volumeOut);
		void setLanguage(unsigned short language);
		static unsigned short languageId(const char* language);
		bool isFound(short index, SoundType type, const char* name, unsigned short lang);

	private:
		unsigned short currentLanguage_;
		static const int MAX_SOUNDS = 300;
		struct Sound {
			unsigned short language_;
			SoundType type_;
			se_core::String* nameC_;
			const char* name_;
			se_core::String* soundC_;
			se_core::String* filename_;
			FMOD::Sound *sound_;
			float volume_;
			bool shouldLoop_;
		} *sounds_;

		short soundCount_;
	};

}

#endif
