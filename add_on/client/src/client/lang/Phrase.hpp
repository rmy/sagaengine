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

namespace se_client {
	/**
	 * Maintains in-game text in multiple languages.
	 */
	class _SeClientExport Phrase {
	public:
		Phrase();
		~Phrase();
		enum PhraseType { UNDEFINED, MENU_LABEL, ACTION_LABEL, THING_LABEL, THING_DESCRIPTION, SPEECH, TYPE_COUNT };

		void addPhrase(unsigned short language, PhraseType type, se_core::String* name, se_core::String* phrase);
		unsigned short findPhrase(PhraseType type, const char* name);
		const char *getPhrase(PhraseType type, const char* name);
		void setLanguage(unsigned short language);
		unsigned short currentLanguage() { return currentLanguage_; }
		void clampToSupportedLanguage();
		bool isSupportedLanguage();
		static unsigned short languageId(const char* language);
		static short typeIdOfName(const char* name);

		void addLanguage(unsigned short id, const char* name);
		void addLanguage(const char* id, const char* name);

		struct Language {
			const char* name_;
			unsigned short id_;
		};
		int supportedLanguageCount() { return supportedLanguageCount_; }
		Language* supportedLanguage(int index) { return &supportedLanguages_[ index ]; }

	private:
		unsigned short currentLanguage_;
		static const int MAX_PHRASES = 1024;
		unsigned short* languages_;
		PhraseType* types_;
		se_core::String** namesC_;
		se_core::String** phrasesC_;
		const char** names_;
		const char** phrases_;
		unsigned short phraseCount_;

		static const int MAX_LANGUAGES = 32;
		int supportedLanguageCount_;
		Language* supportedLanguages_;
	};

}

#endif
