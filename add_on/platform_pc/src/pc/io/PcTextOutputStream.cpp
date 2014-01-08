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
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace se_core;

namespace se_pc {

	PcTextOutputStream
	::PcTextOutputStream() : out_(0) {
		sourceFilename_[0] = 0;
	}


	PcTextOutputStream
	::PcTextOutputStream(const char* directory, const char* filename) {
		open(directory, filename);
	}


	PcTextOutputStream
	::~PcTextOutputStream() {
		close();
	}


	void PcTextOutputStream
	::writeHeaderCode(unsigned int code) {
		const char* buffer = reinterpret_cast<const char*>(&code);
		sprintf(tmp_, "%c%c%c%c\n", buffer[3], buffer[2], buffer[1], buffer[0]);
		writeToken(tmp_);
	}

	void PcTextOutputStream
	::writeInfoCode(int code) {
		sprintf(tmp_, "\n%c ", code);
		writeToken(tmp_);
	}


	void PcTextOutputStream
	::writeInt(int value) {
		sprintf(tmp_, "%d ", value);
		writeToken(tmp_);
	}

	void PcTextOutputStream
	::writeShort(unsigned short value) {
		sprintf(tmp_, "%d ", value);
		writeToken(tmp_);
	}

	void PcTextOutputStream
	::writeByte(unsigned char value) {
		sprintf(tmp_, "%d ", value);
		writeToken(tmp_);
	}


	void PcTextOutputStream
	::writeFloat(float value) {
		sprintf(tmp_, "%f ", value);
		writeToken(tmp_);
	}


	void PcTextOutputStream
	::writeToken(const char* string) {
		Assert(out_);
		fputs(string, out_);
	}

	void PcTextOutputStream
	::writeString(const char* string) {
		writeToken(string);
		writeToken(" ");
	}



	bool PcTextOutputStream
	::open(const char* directory, const char* filename) {
		Assert(strlen(directory) + strlen(filename) + 2 < sizeof(sourceFilename_));
		sprintf(sourceFilename_, "%s/%s", directory, filename);
		LogDetail("Open output stream: " << sourceFilename_);
		out_ = fopen(sourceFilename_, "wt");

		return out_ != 0;
	}


	void PcTextOutputStream
	::close() {
		fclose(out_);
	}
}

