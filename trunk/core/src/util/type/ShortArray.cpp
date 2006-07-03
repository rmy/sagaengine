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


#include "ShortArray.hpp"

namespace se_core {

	ShortArray
	::ShortArray() : data_(0), doDestroy_(false) {
	}


	ShortArray
	::~ShortArray() {
		if(doDestroy_) {
			delete[] data_;
		}
	}


	void ShortArray
	::set(unsigned short* data, bool doDestroy) {
		if(doDestroy_) {
			delete[] data_;
		}
		data_ = data;
		doDestroy_ = doDestroy;
	}


	void ShortArray
	::set(unsigned short* data) {
		if(doDestroy_) {
			delete[] data_;
		}
		data_ = data;
		doDestroy_ = true;
	}

	void ShortArray
	::set(const unsigned short* data) {
		if(doDestroy_) {
			delete[] data_;
		}
		data_ = data;
		doDestroy_ = false;
	}

}
