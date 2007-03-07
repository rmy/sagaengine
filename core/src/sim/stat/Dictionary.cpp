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
#include "../config/sim_config.hpp"
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


	short Dictionary
	::highestId(short type) const {
		int v = -1;
		for(int i = 0; i < entryCount_; ++i) {
			if(type == entries_[i]->type_ && v < entries_[i]->id_) {
				v = entries_[i]->id_;
			}
		}
		return v;
	}


	void Dictionary
	::add(const DictionaryEntry* entry) {
		if(checkName(entry->type_, entry->name_, entry->id_)) {
			// Duplicate
			return;
		}
		Assert(entryCount_ < MAX_ENTRIES);
		if(entry->type_ == DE_DICTIONARY_TYPE) {
			LogMsg("Registered dictionary with name " << entry->name_ << ", type " << entry->type_ << " and id " << entry->id_);
		}
		entries_[ entryCount_++ ] = entry;
		LogMsg(entryCount_);
	}


	short Dictionary
	::id(short type, const char* name) {
		//LogMsg(entryCount_);
		for(int i = 0; i < entryCount_; ++i) {
			//LogMsg("Read dictionary entry: " << entries_[i]->type_ << ", " << entries_[i]->name_);
			//LogMsg(i << ", " << entries_[i]->type_ << ", " << entries_[i]->name_ << " == " << entries_[i]->id_);
			if(entries_[i]->type_ == type && strcmp(entries_[i]->name_, name) == 0) {
				return entries_[i]->id_;
			}
		}
		LogFatal("Unknown dictionary entry of type" << type << ": " << name);
		return -1;
	}


	bool Dictionary
	::checkName(short type, const char* name, short id) {
		for(int i = 0; i < entryCount_; ++i) {
			if(entries_[i]->type_ == type && strcmp(entries_[i]->name_, name) == 0) {
				if(id == entries_[i]->id_) {
					LogMsg("Dictionary duplicate: " << type << ", " << name);
					return true;
				}
				else {
					LogWarning("Same name 2 ids: " << type << ", " << name);
				}
			}
		}
		return false;
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


	bool Dictionary
	::hasId(short type, short id) const {
		for(int i = 0; i < entryCount_; ++i) {
			if(entries_[i]->type_ == type && entries_[i]->id_ == id) {
				return true;
			}
		}

		return false;
	}


}
