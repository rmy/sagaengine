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


#include "PcBinaryInputStream.hpp"
#include "util/type/all.hpp"


using namespace se_core;

namespace se_pc {
	PcBinaryInputStream
	::PcBinaryInputStream() {
	}


 	PcBinaryInputStream
	::PcBinaryInputStream(const char* directory, const char* filename)
		: file_(directory, filename) {
	}


	PcBinaryInputStream
	::~PcBinaryInputStream() {
		file_.close();
	}


	int PcBinaryInputStream
	::readInt() {
		return file_.readLong();
	}


	unsigned short PcBinaryInputStream
	::readShort() {
		return file_.readShort();
	}


	unsigned int PcBinaryInputStream
	::readHeaderCode() {
		unsigned int code = file_.readByte() * (1L << 24) + file_.readByte() * (1L << 16)
			+ file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	unsigned short PcBinaryInputStream
	::readLanguageCode() {
		unsigned int code = file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	int PcBinaryInputStream
	::readPhraseType() {
		unsigned int code = file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	float PcBinaryInputStream
	::readFloat() {
		return file_.readFloat();
	}


	void PcBinaryInputStream
	::readString(String& dest) {
		file_.readString(dest);
	}


	void PcBinaryInputStream
	::readString(char* dest, short maxLen) {
		file_.readString(dest, maxLen);
	}


	int PcBinaryInputStream
	::readInfoCode() {
		return file_.readByte();
	}


	void PcBinaryInputStream
	::readShortArray(ShortArray& dest, int size) {
		file_.readShortArray(dest, size);
	}


	void PcBinaryInputStream
	::readByteArray(ByteArray& dest, int size) {
		file_.readByteArray(dest, size);
	}


	void PcBinaryInputStream
	::readCharArray(String& dest, int size) {
		file_.readCharArray(dest, size);
	}


	bool PcBinaryInputStream
	::eof() {
		return file_.eof();
	}

}

