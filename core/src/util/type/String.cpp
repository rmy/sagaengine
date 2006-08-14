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
	::copy(const char* s) {
		if(doDestroy_) delete[] data_;
		char* buffer = new char[strlen(s) + 1];
		strcpy(buffer, s);
		data_ = buffer;
		doDestroy_ = true;
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
}
