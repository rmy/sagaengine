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


#include "String.hpp"
#include "../error/Log.hpp"
#include <cstring>

namespace se_core {
	String
	::String() : data_(0), doDestroy_(false) {
	}

	String
	::String(const char* data) : data_(data), doDestroy_(false) {
	}

	String
	::String(char* data) : data_(data), doDestroy_(true) {
	}


	String
	::String(const char* name, int toPos) : data_(0), doDestroy_(false) {
		copy(name, toPos);
	}

	String
	::~String() {
		if(doDestroy_) delete[] data_;
	}

	void String
	::set(char* data) {
		if(doDestroy_) delete[] data_;
		data_ = data;
		doDestroy_ = true;
	}


	void String
	::set(const char* data) {
		if(doDestroy_) delete[] data_;
		data_ = data;
		doDestroy_ = false;
	}


	void String
	::reset() {
		if(doDestroy_) delete[] data_;
		data_ = 0;
		doDestroy_ = false;
	}

	void String
	::copy(const char* s) {
		if(doDestroy_) delete[] data_;
		char* buffer = new char[strlen(s) + 1];
		strcpy(buffer, s);
		data_ = buffer;
		doDestroy_ = true;
	}


	void String
	::copy(const char* s, int toPos) {
		int len = strlen(s);
		if(toPos <= 0) {
			len += toPos;
			if(len < 0)
				len = 0;
		}
		else if(toPos < len) {
			len = toPos;
		}
		if(doDestroy_) delete[] data_;
		char* buffer = new char[len + 1];
		strncpy(buffer, s, len);
		buffer[ len ] = 0;
		data_ = buffer;
		doDestroy_ = true;
	}


	const char* String
	::ext(char delimiter, bool mustFind) const {
		const char* ch = data_, *ret = 0;

		while(*ch != 0) {
			if(*ch == delimiter) {
				ret = ch + 1;
			}
			++ch;
		}
		AssertFatal(!mustFind  || ret, "Couldn't find delimiter '" << delimiter << "' in " << get());
		return ret;
	}

	char* String
	::copyValue() const {
		char* v = new char[strlen(data_) + 1];
		strcpy(v, data_);
		return v;
	}


	bool String
	::isEmpty() const {
		return (data_ == 0 || data_[0] == 0);
	}


	void String
	::append(const char* s) {
		char* v = new char[strlen(data_) + strlen(s) + 1];
		strcpy(v, data_);
		strcat(v, s);
		set(v);
	}


	int String
	::compare(const String& s) const {
		if(get() == s.get())
			return 0;
		if(get() == 0)
			return -1;
		if(s.get() == 0)
			return 1;
		return strcmp(get(), s.get());
	}


	int String
	::compare(const char* s) const {
		if(get() == s)
			return 0;
		if(get() == 0)
			return -1;
		if(s == 0)
			return 1;
		return strcmp(get(), s);
	}


	bool String
	::equals(const String& s) const {
		return (compare(s) == 0);
	}


	bool String
	::equals(const char* s) const {
		return (compare(s) == 0);
	}

	int String
	::len() const {
		if(!data_)
			return 0;
		return strlen(data_);
	}

}
