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


#include "Dictionary.hpp"
#include "util/error/Log.hpp"
#include <cstring>

namespace se_core {

	Dictionary
	::Dictionary()
			: entryCount_(0)
			  , entries_(new const DictionaryEntry*[ MAX_ENTRIES ]) {
	}


	Dictionary
	::~Dictionary() {
		delete[] entries_;
	}


	void Dictionary
	::add(const DictionaryEntry* entry) {
		Assert(entryCount_ < MAX_ENTRIES);
		//LogMsg(entryCount_ << " - " << entry->type_ << ": " << entry->name_ << " = " << entry->id_);
		entries_[ entryCount_++ ] = entry;
	}


	short Dictionary
	::id(short type, const char* name) {
		//LogMsg(entryCount_);
		for(int i = 0; i < entryCount_; ++i) {
			//LogMsg(i << ", " << entries_[i]->type_ << ", " << entries_[i]->name_ << " == " << entries_[i]->id_);
			if(entries_[i]->type_ == type && strcmp(entries_[i]->name_, name) == 0) {
				//LogMsg("Read dictionary entry: " << type << ", " << name);
				return entries_[i]->id_;
			}
		}
		LogFatal("Unkown dictionary entry: " << type << ", " << name);
		return -1;
	}


	const char* Dictionary
	::name(short type, short id) {
		for(int i = 0; i < entryCount_; ++i) {
			if(entries_[i]->type_ == type && entries_[i]->id_ == id) {
				return entries_[i]->name_;
			}
		}

		LogFatal("Unkown dictionary entry: " << type << ", " << id);
		return 0;
	}

}
