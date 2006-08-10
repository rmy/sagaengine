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


#ifndef Phrase_hpp
#define Phrase_hpp

#include "util/type/util_type.hpp"

namespace se_core {
	class _SeClientExport Phrase {
	public:
		Phrase();
		virtual ~Phrase();
		enum PhraseType { UNDEFINED, MENU_LABEL, ACTION_LABEL, THING_LABEL, THING_DESCRIPTION, SPEECH, TYPE_COUNT };
		static const unsigned short ENGLISH = 'U' * 256 + 'K';
		static const unsigned short NORWEGIAN = 'N' * 256 + 'O';
		static const unsigned short FRENCH = 'F' * 256 + 'R';

		void addPhrase(unsigned short language, PhraseType type, String* name, String* phrase);
		unsigned short findPhrase(PhraseType type, const char* name);
		const char *getPhrase(PhraseType type, const char* name);
		void setLanguage(unsigned short language);
		static unsigned short languageId(const char* language);
		static short typeIdOfName(const char* name);

	private:
		unsigned short currentLanguage_;
		static const int MAX_PHRASES = 300;
		unsigned short languages_[ MAX_PHRASES ];
		PhraseType types_[ MAX_PHRASES ];
		String* namesC_[ MAX_PHRASES ];
		String* phrasesC_[ MAX_PHRASES ];
		const char* names_[ MAX_PHRASES ];
		const char* phrases_[ MAX_PHRASES ];
		unsigned short phraseCount_;
	};

}

#endif
