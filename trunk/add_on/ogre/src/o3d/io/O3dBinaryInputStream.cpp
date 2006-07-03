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


#include "O3dBinaryInputStream.hpp"
#include "util/type/all.hpp"


using namespace se_core;

namespace se_ogre {
	O3dBinaryInputStream
	::O3dBinaryInputStream() {
	}


 	O3dBinaryInputStream
	::O3dBinaryInputStream(const char* directory, const char* filename)
		: file_(directory, filename) {
	}


	O3dBinaryInputStream
	::~O3dBinaryInputStream() {
		file_.close();
	}


	int O3dBinaryInputStream
	::readInt() {
		return file_.readLong();
	}


	unsigned short O3dBinaryInputStream
	::readShort() {
		return file_.readShort();
	}


	unsigned int O3dBinaryInputStream
	::readHeaderCode() {
		unsigned int code = file_.readByte() * (1L << 24) + file_.readByte() * (1L << 16)
			+ file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	unsigned short O3dBinaryInputStream
	::readLanguageCode() {
		unsigned int code = file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	int O3dBinaryInputStream
	::readPhraseType() {
		unsigned int code = file_.readByte() * (1L << 8) + file_.readByte();
		return code;
	}


	float O3dBinaryInputStream
	::readFloat() {
		return file_.readFloat();
	}


	void O3dBinaryInputStream
	::readString(String& dest) {
		file_.readString(dest);
	}


	void O3dBinaryInputStream
	::readString(char* dest, short maxLen) {
		file_.readString(dest, maxLen);
	}


	int O3dBinaryInputStream
	::readInfoCode() {
		return file_.readByte();
	}


	void O3dBinaryInputStream
	::readShortArray(ShortArray& dest, int size) {
		file_.readShortArray(dest, size);
	}


	void O3dBinaryInputStream
	::readByteArray(ByteArray& dest, int size) {
		file_.readByteArray(dest, size);
	}


	void O3dBinaryInputStream
	::readCharArray(String& dest, int size) {
		file_.readCharArray(dest, size);
	}


	bool O3dBinaryInputStream
	::eof() {
		return file_.eof();
	}

}

