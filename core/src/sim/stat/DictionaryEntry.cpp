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


#include "DictionaryEntry.hpp"
#include "Dictionary.hpp"
#include "../schema/all.hpp"
#include "util/error/Log.hpp"
#include <cstring>

namespace se_core {

	DictionaryEntry
	::DictionaryEntry(short type, short id, const char* name, bool doCopyName) {
		type_ = type;
		id_ = id;
		if(doCopyName) {
			char* n = new char[ strlen(name) + 1 ];
			strcpy(n, name);
			name_ = n;
			doDelete_ = true;
		}
		else {
			name_ = name;
			doDelete_ = false;
		}
		addToDictionary();
	}


	DictionaryEntry
	::DictionaryEntry(short type, short id, char* name, bool doCopyName) {
		type_ = type;
		id_ = id;
		if(doCopyName) {
			char* n = new char[ strlen(name) + 1 ];
			strcpy(n, name);
			name_ = n;
			doDelete_ = true;
		}
		else {
			name_ = name;
			doDelete_ = true;
		}
		addToDictionary();
	}


	DictionaryEntry
	::DictionaryEntry(short type, const char* name, bool doCopyName) {
		type_ = type;
		id_ = Dictionary::hash(name);
		if(SimSchema::dictionary().hasId(type, id_)) {
			Assert(strcmp(name, SimSchema::dictionary().name(type, id_)) == 0)
		}

		if(doCopyName) {
			char* n = new char[ strlen(name) + 1 ];
			strcpy(n, name);
			name_ = n;
			doDelete_ = true;
		}
		else {
			name_ = name;
			doDelete_ = true;
		}
		addToDictionary();
	}


	DictionaryEntry
	::~DictionaryEntry() {
		SimSchema::dictionary().remove(this);
		if(doDelete_) delete[] name_;
	}


	void DictionaryEntry
	::addToDictionary() {
		SimSchema::dictionary().add(this);
	}

}
