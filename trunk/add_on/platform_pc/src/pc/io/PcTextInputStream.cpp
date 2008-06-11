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
#include "PcTextInputStream.hpp"
#include "PcFile.hpp"
#include "util/type/all.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace se_core;

namespace se_pc {

	PcTextInputStream
	::PcTextInputStream() {
		fileContents_ = 0;
	}


	PcTextInputStream
	::PcTextInputStream(const char* directory, const char* filename) {
		Assert(strlen(directory) + strlen(filename) + 2 < sizeof(sourceFilename_));
		sprintf(sourceFilename_, "%s/%s", directory, filename);

		FILE* fd = fopen(sourceFilename_, "rb");
		if(!fd) {
			LogFatal("Couldn't open file: " << sourceFilename_);
		}
		fseek(fd, 0, SEEK_END);
		contentsLength_ = ftell(fd);
		rewind(fd);
		fileContents_ = new char[contentsLength_ + 1];
		size_t bytesRead = fread(fileContents_, contentsLength_, 1, fd);
		fclose(fd);

		fileContents_[ contentsLength_ ] = 0;
		bufferIndex_ = 0;
	}


	PcTextInputStream
	::~PcTextInputStream() {
		if (fileContents_)
			delete[] fileContents_;
	}


	void PcTextInputStream
	::nextToken() {
		// Skips whitespace if any to first non-whitespace character
		bool foundWhiteSpace = false;
		if(bufferIndex_ >= contentsLength_)
			return;
		do {
			switch(fileContents_[bufferIndex_]) {
			case ' ':
			case '\r':
			case '\t':
			case '\n':
				foundWhiteSpace = true;
				bufferIndex_++;
				break;
			case '#':
				// Comment out rest of line
				while(bufferIndex_ < contentsLength_ && fileContents_[bufferIndex_] != '\n' && fileContents_[bufferIndex_] != '\r') {
					++bufferIndex_;
				}
				foundWhiteSpace = true;
				break;
			case 0:
				return;
			default:
				if(foundWhiteSpace) return;
			}
		} while (foundWhiteSpace && bufferIndex_ < contentsLength_);
	}


	int PcTextInputStream
	::readInt() {
		readString();
		int v = atoi(tmpBuffer_);
		return v;
	}


	unsigned int PcTextInputStream
	::readHeaderCode() {
		// Skip unicode byte order mark (BOM)
		if((unsigned char)fileContents_[bufferIndex_] == 0xef) {
			LogWarning("Skipping unicode BOM");
			bufferIndex_ += 3;
		}
		unsigned int code =((unsigned char)fileContents_[bufferIndex_++] << 24);
		code+=((unsigned char)fileContents_[bufferIndex_++] << 16);
		code+=((unsigned char)fileContents_[bufferIndex_++] <<  8);
		code+=((unsigned char)fileContents_[bufferIndex_++]);
		nextToken();
		return code;
	}


	unsigned short PcTextInputStream
	::readLanguageCode() {
		unsigned int code = (fileContents_[bufferIndex_++] <<  8);
		code+=  fileContents_[bufferIndex_++];
		nextToken();
		return code;
	}


	float PcTextInputStream
	::readFloat() {
		readString();
		float v = static_cast<float>(atof(tmpBuffer_));
		return v;
	}


	void PcTextInputStream
	::readLine(String& dest) {
		int i=0;
		while(bufferIndex_ < contentsLength_ && fileContents_[bufferIndex_] != '\n' && fileContents_[bufferIndex_] != '\r') {
			tmpBuffer_[i++] = fileContents_[bufferIndex_++];
		}
		tmpBuffer_[i] = 0;
		for(i=0; tmpBuffer_[i]; i++) {
			if(tmpBuffer_[i] == '#') {
				tmpBuffer_[i]=0; 
				break;
			}
		}
		dest.copy(tmpBuffer_);
		nextToken();
	}


	void PcTextInputStream
	::readLine(char* dest, short maxLen) {
		int i=0;
		while(bufferIndex_ < contentsLength_ && fileContents_[bufferIndex_] != '\n' && fileContents_[bufferIndex_] !=' \r') {
			dest[i++]=fileContents_[bufferIndex_++];
		}
		dest[i] = 0;
		for (i=0;dest[i];i++) {
			if(dest[i]=='#') {dest[i]=0; break;}
		}
		nextToken();
	}


	char* PcTextInputStream
	::readString() {
		unsigned short i = 0;
		while(bufferIndex_ < contentsLength_) {
			char c = fileContents_[bufferIndex_];
			if(c == ' ' || c == '\n' || c == '\t' || c == '\r' || c=='#') {
				break;
			}

			tmpBuffer_[i++] = fileContents_[bufferIndex_++];
			if(i >= sizeof(tmpBuffer_)-1) {
				LogFatal("String too long");
				--i;
				break;
			}
		}
		tmpBuffer_[i] = 0;
		nextToken();
		return tmpBuffer_;
	}


	void PcTextInputStream
	::readString(char* dest, short maxLen) {
		char *r = readString();
		dest[ maxLen - 1 ] = 0;
		strncpy(dest, r, maxLen - 1);
	}


	void PcTextInputStream
	::readString(String& dest) {
		char *r = readString();
		dest.copy(r);
	}


	int PcTextInputStream
	::readInfoCode() {
		int v = fileContents_[bufferIndex_++];
		// Eof not always working, so next token after
		// Q may hang. This means that Q cannot be used
		// as info code in sub-scripts.
		//if(v != 'Q')
		nextToken();
		return v;
	}


	int PcTextInputStream
	::readPhraseType() {
		return readDictionaryWord(DE_PHRASE_TYPE);
	}


	int PcTextInputStream
	::readThingType() {
		return readDictionaryWord(DE_THING_TYPE);
	}


	void PcTextInputStream
	::readShortArray(ShortArray& dest, int size) {
		unsigned short* array = new unsigned short[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void PcTextInputStream
	::readByteArray(ByteArray& dest, int size) {
		unsigned char* array = new unsigned char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void PcTextInputStream
	::readCharArray(String& dest, int size) {
		char* array = new char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	bool PcTextInputStream
	::eof() {
		return bufferIndex_ >= contentsLength_;
	}

}

