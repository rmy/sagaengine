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


#ifndef engine_stat_Dictionary_hpp
#define engine_stat_Dictionary_hpp

#include "DictionaryEntry.hpp"

namespace se_core {
	class _SeCoreExport Dictionary {
	public:
		Dictionary();
		virtual ~Dictionary();
		short id(short type, const char* name);
		const char* name(short type, short id);
		short highestId(short type) const;
		bool hasId(short type, short id) const;

		const DictionaryEntry* find(short type, const char* name);
		static short hash(const char* name);
		bool hasEntry(short type, const char* name, short id) {
			return checkName(type, name, id);
		}

	private:
		bool checkName(short type, const char* name, short id);

		friend class DictionaryEntry;
		bool add(const DictionaryEntry* entry);
		void remove(const DictionaryEntry* entry);

		short entryCount_;
		const static int MAX_ENTRIES = 2048;
		const DictionaryEntry** entries_;
	};

}

#endif
