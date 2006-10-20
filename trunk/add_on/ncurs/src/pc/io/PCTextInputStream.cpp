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


#include "PCTextInputStream.hpp"
#include "PCFile.hpp"
#include "util/type/all.hpp"
#include "client/lang/Phrase.hpp"
#include "sim/pos/Pos.hpp"
#include <cstring>
#include <cstdlib>

using namespace se_core;


namespace se_ncurs {
	PCTextInputStream
	::PCTextInputStream() {
	}


	PCTextInputStream
	::PCTextInputStream(const char* directory, const char* filename)
		: file_(directory, filename) {
	}


	PCTextInputStream
	::~PCTextInputStream() {
		file_.close();
	}


	void PCTextInputStream
	::nextToken() {
		bool foundWhiteSpace = false;
		DebugExec(short i = 128);
		while(!eof()) {
			unsigned char ch = file_.nextByte();
			//LogMsg((sprintf(log_msg(), "Code: 0x%02x '%c'", ch, ch), log_msg()));
			switch(ch) {
			case ' ':
			case '\r':
			case '\t':
			case '\n':
				foundWhiteSpace = true;
				break;
			default:
				if(foundWhiteSpace) {
					return;
				}
			}
			file_.readByte();
			DbgAssert(--i >= 0 && "Suspicious with this much whitespace before next token.");
		}
	}


	int PCTextInputStream
	::readInt() {
		readString();
		int v = atoi(buffer_);
		return v;
	}


	unsigned int PCTextInputStream
	::readHeaderCode() {
		buffer_[0] = file_.readByte();
		buffer_[1] = file_.readByte();
		buffer_[2] = file_.readByte();
		buffer_[3] = file_.readByte();

		unsigned int code = buffer_[3] + buffer_[2] * 256 + buffer_[1] * 65536 + buffer_[0] * 65536 * 256;
		nextToken();
		return code;
	}


	unsigned short PCTextInputStream
	::readLanguageCode() {
		buffer_[0] = file_.readByte();
		buffer_[1] = file_.readByte();
		unsigned int code = buffer_[1] + buffer_[0] * 256;
		nextToken();
		return code;
	}


	float PCTextInputStream
	::readFloat() {
		readString();
		float v = static_cast<float>(atof(buffer_));
		return v;
	}


	void PCTextInputStream
	::readLine(String& dest) {
		file_.readLine(dest);
	}


	void PCTextInputStream
	::readLine(char* dest, short maxLen) {
		file_.readLine(dest, maxLen);
	}


	char* PCTextInputStream
	::readString() {
		unsigned short i = 0;
		while(!eof()) {
			char c = file_.nextByte();
			if(c == ' ' || c == '\n' || c == '\t' || c == '\r') {
				break;
			}

			buffer_[i++] = file_.readByte();
			if(i >= sizeof(buffer_)) {
				LogFatal("String too long");
				--i;
				break;
			}
		}
		buffer_[i] = 0;
		//LogMsg(buffer_);
		nextToken();
		return buffer_;
	}


	void PCTextInputStream
	::readString(char* dest, short maxLen) {
		char *r = readString();
		dest[ maxLen - 1 ] = 0;
		strncpy(dest, r, maxLen - 1);
	}


	void PCTextInputStream
	::readString(String& dest) {
		char *r = readString();
		char* result = new char[ strlen(r) + 1 ];
		strcpy(result, r);
		dest.set(result);
	}


	int PCTextInputStream
	::readInfoCode() {
		int v = file_.readByte();
		// Eof not always working, so next token after
		// Q may hang. This means that Q cannot be used
		// as info code in sub-scripts.
		if(v != 'Q')
			nextToken();
		return v;
	}


	int PCTextInputStream
	::readThingType() {
		return readDictionaryWord(DE_THING_TYPE);
	}


	int PCTextInputStream
	::readPhraseType() {
		char* name = readString();
		int id = static_cast<int>(Phrase::typeIdOfName(name));
		return id;
	}


	void PCTextInputStream
	::readShortArray(ShortArray& dest, int size) {
		unsigned short* array = new unsigned short[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void PCTextInputStream
	::readByteArray(ByteArray& dest, int size) {
		unsigned char* array = new unsigned char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void PCTextInputStream
	::readCharArray(String& dest, int size) {
		char* array = new char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	bool PCTextInputStream
	::eof() {
		return file_.eof();
	}

}

