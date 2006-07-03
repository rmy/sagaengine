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


#include "Phrase.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstring>


namespace se_core {
	Phrase
	::Phrase() : currentLanguage_(NORWEGIAN) {
		//setLanguage(ENGLISH);
	}

	Phrase
	::~Phrase() {
		while(phraseCount_ > 0) {
			--phraseCount_;
			delete namesC_[ phraseCount_ ];
			delete phrasesC_[ phraseCount_ ];
		}
	}


	void Phrase
	::setLanguage(unsigned short language) {
		currentLanguage_ = language;
		//if(language == FRENCH) currentLanguage_ = NORWEGIAN;
	}

	void Phrase
	::addPhrase(unsigned short language, PhraseType type, String* nameC, String* phraseC) {
		Assert(phraseCount_ < MAX_PHRASES);

		const char* name = nameC->get();
		const char* phrase = phraseC->get();
		unsigned short cl = currentLanguage_;
		currentLanguage_ = language;
		unsigned short index = findPhrase(type, name);
		if(index < phraseCount_
		   && (languages_[ index ] != language
			   || types_[ index ] != type || strcmp(names_[ index ], name) != 0)
			) {
			for(int i = phraseCount_; i > index; --i) {
				languages_[ i ] = languages_[ i - 1 ];
				types_[ i ] = types_[ i - 1 ];
				names_[ i ] = names_[ i - 1 ];
				phrases_[ i ] = phrases_[ i - 1 ];

				namesC_[ i ] = namesC_[ i - 1 ];
				phrasesC_[ i ] = phrasesC_[ i - 1 ];
			}
		}
		languages_[ index ] = language;
		types_[ index ] = type;
		namesC_[ index ] = nameC;
		phrasesC_[ index ] = phraseC;
		names_[ index ] = name;
		phrases_[ index ] = phrase;
		++phraseCount_;

		currentLanguage_ = cl;
	}


	unsigned short Phrase
	::findPhrase(PhraseType type, const char* name) {
		unsigned short min = 0;
		unsigned short max = phraseCount_;
		unsigned short middle;
		while(max > min) {
			middle = (max + min) / 2;
			if(languages_[ middle ] < currentLanguage_
				|| (languages_[ middle ] == currentLanguage_ && types_[ middle ] < type)
				|| (languages_[ middle ] == currentLanguage_ && types_[ middle ] == type && strcmp(names_[middle], name) < 0)) {
				min = middle + 1;
			}
			else {
				max = middle;
			}
		}
		return min;
	}


	const char *Phrase
	::getPhrase(PhraseType type, const char* name) {
		return phrases_[ findPhrase(type, name) ];
	}


	unsigned short Phrase
	::languageId(const char* name) {
		return name[0] * 256 + name[1];
	}


	short Phrase
	::typeIdOfName(const char* name) {
		if(strcmp(name, "MENU_LABEL") == 0)
			return MENU_LABEL;
		if(strcmp(name, "THING_LABEL") == 0)
			return THING_LABEL;
		if(strcmp(name, "ACTION_LABEL") == 0)
			return ACTION_LABEL;
		if(strcmp(name, "THING_DESCRIPTION") == 0)
			return THING_DESCRIPTION;
		if(strcmp(name, "SPEECH") == 0)
			return SPEECH;
		LogFatal(name);
		return UNDEFINED;
	}

}
