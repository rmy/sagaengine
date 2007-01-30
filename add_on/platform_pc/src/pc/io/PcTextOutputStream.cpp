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


#include "PcPre.hpp"
#include "PcTextOutputStream.hpp"
#include "PcFile.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace se_core;

namespace se_pc {

	PcTextOutputStream
	::PcTextOutputStream() {
		sourceFilename_[0] = 0;
	}


	PcTextOutputStream
	::PcTextOutputStream(const char* directory, const char* filename) {
		Assert(strlen(directory) + strlen(filename) + 2 < sizeof(sourceFilename_));
		sprintf(sourceFilename_, "%s/%s", directory, filename);
	}


	PcTextOutputStream
	::~PcTextOutputStream() {
	}


	void PcTextOutputStream
	::writeHeaderCode(unsigned int code) {
	}

	void PcTextOutputStream
	::writeInfoCode(int code) {
	}


	void PcTextOutputStream
	::writeInt(int value) {
	}

	void PcTextOutputStream
	::writeShort(unsigned short value) {
	}

	void PcTextOutputStream
	::writeByte(unsigned char value) {
	}


	void PcTextOutputStream
	::writeFloat(float value) {
	}

	void PcTextOutputStream
	::writeString(const char* string) {
	}


}

