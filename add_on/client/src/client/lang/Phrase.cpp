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
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/config/sim_config.hpp"
#include <cstring>

using namespace se_core;

namespace se_client {
	Phrase
	::Phrase() : currentLanguage_(0), phraseCount_(0), supportedLanguageCount_(0) {
		languages_ = new unsigned short[ MAX_PHRASES ];
		types_ = new PhraseType[ MAX_PHRASES ];
		namesC_ = new se_core::String*[ MAX_PHRASES ];
		phrasesC_ = new se_core::String*[ MAX_PHRASES ];
		names_ = new const char*[ MAX_PHRASES ];
		phrases_ = new const char*[ MAX_PHRASES ];

		supportedLanguages_ = new Language[ MAX_LANGUAGES ];

		static DictionaryEntry ph1(DE_PHRASE_TYPE, MENU_LABEL, "MENU_LABEL");
		static DictionaryEntry ph2(DE_PHRASE_TYPE, ACTION_LABEL, "ACTION_LABEL");
		static DictionaryEntry ph3(DE_PHRASE_TYPE, THING_LABEL, "THING_LABEL");
		static DictionaryEntry ph4(DE_PHRASE_TYPE, THING_DESCRIPTION, "THING_DESCRIPTION");
		static DictionaryEntry ph5(DE_PHRASE_TYPE, SPEECH, "SPEECH");
	}


	Phrase
	::~Phrase() {
		while(phraseCount_ > 0) {
			--phraseCount_;
			delete namesC_[ phraseCount_ ];
			delete phrasesC_[ phraseCount_ ];
		}
		delete[] languages_;
		delete[] types_;
		delete[] namesC_;
		delete[] phrasesC_;
		delete[] names_;
		delete[] phrases_;

		delete[] supportedLanguages_;
	}


	void Phrase
	::setLanguage(unsigned short language) {
		currentLanguage_ = language;
	}

	void Phrase
	::clampToSupportedLanguage() {
		for(int i = 0; i < supportedLanguageCount_; ++i) {
			if(supportedLanguages_[i].id_ == currentLanguage_)
				return;
		}
		Assert(supportedLanguageCount_ > 0);
		currentLanguage_ = supportedLanguages_[0].id_;
	}


	void Phrase
	::addPhrase(unsigned short language, PhraseType type, String* nameC, String* phraseC) {
		Assert(phraseCount_ < MAX_PHRASES);

		const char* name = nameC->get();
		const char* phrase = phraseC->get();
		unsigned short cl = currentLanguage_;
		currentLanguage_ = language;
		unsigned short index = findPhrase(type, name);

		for(int i = phraseCount_; i > index; --i) {
			languages_[ i ] = languages_[ i - 1 ];
			types_[ i ] = types_[ i - 1 ];
			names_[ i ] = names_[ i - 1 ];
			phrases_[ i ] = phrases_[ i - 1 ];

			namesC_[ i ] = namesC_[ i - 1 ];
			phrasesC_[ i ] = phrasesC_[ i - 1 ];
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
		int index = findPhrase(type, name);
		Assert(index >= 0 && index < phraseCount_);
		if(languages_[ index ] == currentLanguage_ && types_[ index ] == type && strcmp(names_[index], name) == 0) {
			return phrases_[ index ];
		}

		return name;
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


	void Phrase
	::addLanguage(unsigned short id, const char* name) {
		Assert(supportedLanguageCount_ < MAX_LANGUAGES);
		supportedLanguages_[ supportedLanguageCount_ ].name_ = name;
		supportedLanguages_[ supportedLanguageCount_ ].id_ = id;
		++supportedLanguageCount_;
	}


	void Phrase
	::addLanguage(const char* id, const char* name) {
		addLanguage(languageId(id), name);
	}
}
