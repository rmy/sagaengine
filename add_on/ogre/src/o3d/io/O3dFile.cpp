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


#include "O3dFile.hpp"
#include "util/type/all.hpp"
#include "util/error/Log.hpp"
#include <cstdio>
#include <cstring>


using namespace se_core;

namespace se_ogre {
	O3dFile
	::O3dFile() : in_(0) {
	}


	O3dFile
	::O3dFile(const char* directory, const char* filename) : in_(0) {
		open(directory, filename);
	}


	O3dFile
	::~O3dFile() {
		close();
	}


	void O3dFile
	::open(const char* directory, const char* filename) {
		Assert(strlen(directory) + strlen(filename) + 2 < sizeof(fullFilePath_));
		sprintf(fullFilePath_, "%s/%s", directory, filename);
		in_ = fopen(fullFilePath_, "rb");
		LogMsg(fullFilePath_);
		Assert(in_ != 0);
	}


	void O3dFile
	::close() {
		if(in_) fclose(in_);
		in_ = 0;
	}


	void O3dFile
	::readLine(char* dest, short maxLen) {
		dest[0] = 0;
		fgets(dest, maxLen, in_);
		short i = static_cast<short>(strlen(dest) - 1);
		while(i >= 0 && dest[i] == '\n' || dest[i] == '\r') {
			dest[i] = 0;
			--i;
		}

	}


	void O3dFile
	::readLine(String& dest) {
		const short MAX_LEN = 512;
		char buffer[ MAX_LEN ];
		buffer[0] = 0;
		fgets(buffer, MAX_LEN, in_);

		short i = static_cast<short>(strlen(buffer) - 1);
		Assert(i < MAX_LEN);
		while(i >= 0 && (buffer[i] == '\n' || buffer[i] == '\r')) {
			buffer[i] = 0;
			--i;
		}
		short len = i + 1;

		char* d = new char[ len + 1 ];
		strcpy(d, buffer);
		dest.set(d);
	}


	short O3dFile
	::readString(char* dest, short maxLen) {
		--maxLen;
		unsigned char ch = 0;

		short i = 0;
		do {
			ch = readByte();
			dest[i++] = ch;
		} while( i < maxLen && ch != 0);
		dest[i] = 0;
		short len = i;

		while(ch != 0) {
			LogFatal("Too long string in file");
			ch = readByte();
			++i;
		}
		// Quad align (for ROM platforms)
		while(!eof() && (i & 3)) {
			ch = readByte();
			++i;
		}

		return len;
	}


	void O3dFile
	::readString(String& dest) {
		const short MAX_LEN = 512;
		char buffer[ MAX_LEN ];

		short len = readString(buffer, MAX_LEN);
		char* d = new char[ len + 1 ];
		strcpy(d, buffer);
		dest.set(d);
	}


	bool O3dFile
	::eof() {
		return (feof(in_) != 0) && ((ferror(in_) == 0));
	}


	float O3dFile
	::readFloat() {
		float v;
		fread(&v, sizeof(v), 1, in_);
		return v;
	}


	unsigned long O3dFile
	::readLong() {
		long v;
		fread(&v, sizeof(v), 1, in_);
		return v;
	}


	unsigned short O3dFile
	::readShort() {
		unsigned short v;
		fread(&v, sizeof(v), 1, in_);
		return v;
	}


	unsigned char O3dFile
	::readByte() {
		unsigned char v;
		//int n = fread(&v, sizeof(v), 1, in_);
		//Assert(n);
		v = fgetc(in_);
		return v;
	}


	unsigned char O3dFile
	::nextByte() {
		unsigned char v;
		fread(&v, sizeof(v), 1, in_);
		ungetc(v, in_);
		return v;
	}


	void O3dFile
	::readShortArray(ShortArray& dest, int size) {
		unsigned short* array = new unsigned short[ size ];
		fread(array, size, sizeof(unsigned short), in_);
		dest.set(array);
	}


	void O3dFile
	::readByteArray(ByteArray& dest, int size) {
 		unsigned char* array = new unsigned char[ size ];
		fread(array, size, sizeof(unsigned char), in_);
		dest.set(array);
	}


	void O3dFile
	::readCharArray(String& dest, int size) {
 		char* array = new char[ size ];
		fread(array, size, sizeof(char), in_);
		dest.set(array);
	}


	long O3dFile
	::size() {
		LogFatal("Not implemented");
		return 0;
	}


	int O3dFile
	::shortCount() {
		return size() / sizeof(short);
	}


	const char* O3dFile
	::filename() {
		return fullFilePath_;
	}

}
