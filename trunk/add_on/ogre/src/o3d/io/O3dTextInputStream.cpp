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


#include "O3dPre.H"
#include "O3dTextInputStream.hpp"
#include "O3dFile.hpp"
#include <cstring>
#include <cstdlib>
#include <cstdio>

using namespace se_core;


namespace se_ogre {

	O3dTextInputStream
	::O3dTextInputStream() {
		fileContents_ = 0;
	}


	O3dTextInputStream
	::O3dTextInputStream(const char* directory, const char* filename) {
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


	O3dTextInputStream
	::~O3dTextInputStream() {
		if (fileContents_)
			delete[] fileContents_;
	}


	void O3dTextInputStream
	::nextToken() {
		// Skips whitespace if any to first non-whitespace character
		bool foundWhiteSpace = false;
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
			   for(;fileContents_[bufferIndex_]!='\r'&&fileContents_[bufferIndex_]!='\n';bufferIndex_++);
			   foundWhiteSpace=true;
			   break;
		   default:
			   if(foundWhiteSpace) return;
		   }
		} while (foundWhiteSpace);
	}


	int O3dTextInputStream
	::readInt() {
		readString();
		int v = atoi(tmpBuffer_);
		return v;
	}


	unsigned int O3dTextInputStream
	::readHeaderCode() {
		unsigned int code =((unsigned char)fileContents_[bufferIndex_++] << 24);
		code+=((unsigned char)fileContents_[bufferIndex_++] << 16);
		code+=((unsigned char)fileContents_[bufferIndex_++] <<  8);
		code+=((unsigned char)fileContents_[bufferIndex_++]);
		nextToken();
		return code;
	}


	unsigned short O3dTextInputStream
	::readLanguageCode() {
		unsigned int code = (fileContents_[bufferIndex_++] <<  8);
		code+=  fileContents_[bufferIndex_++];
		nextToken();
		return code;
	}


	float O3dTextInputStream
	::readFloat() {
		readString();
		float v = static_cast<float>(atof(tmpBuffer_));
		return v;
	}


	void O3dTextInputStream
	::readLine(String& dest) {
		int i=0;
		while(bufferIndex_<=contentsLength_ && fileContents_[bufferIndex_]!='\n' && fileContents_[bufferIndex_]!='\r') {
			tmpBuffer_[i++]=fileContents_[bufferIndex_++];
		}
		tmpBuffer_[i] = 0;
		for (i=0;tmpBuffer_[i];i++) {
			if(tmpBuffer_[i]=='#') {tmpBuffer_[i]=0; break;}
		}
		dest.copy(tmpBuffer_);
		nextToken();
	}


	void O3dTextInputStream
	::readLine(char* dest, short maxLen) {
		int i=0;
		while(bufferIndex_<=contentsLength_ && fileContents_[bufferIndex_]!='\n' && fileContents_[bufferIndex_]!='\r') {
			dest[i++]=fileContents_[bufferIndex_++];
		}
		dest[i] = 0;
		for (i=0;dest[i];i++) {
			if(dest[i]=='#') {dest[i]=0; break;}
		}
		nextToken();
	}


	char* O3dTextInputStream
	::readString() {
		unsigned short i = 0;
		while(bufferIndex_<contentsLength_) {
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


	void O3dTextInputStream
	::readString(char* dest, short maxLen) {
		char *r = readString();
		dest[ maxLen - 1 ] = 0;
		strncpy(dest, r, maxLen - 1);
	}


	void O3dTextInputStream
	::readString(String& dest) {
		char *r = readString();
		dest.copy(r);
	}


	int O3dTextInputStream
	::readInfoCode() {
		int v = fileContents_[bufferIndex_++];
		// Eof not always working, so next token after
		// Q may hang. This means that Q cannot be used
		// as info code in sub-scripts.
		if(v != 'Q')
			nextToken();
		return v;
	}


	int O3dTextInputStream
	::readPhraseType() {
		char* name = readString();
		int id = static_cast<int>(Phrase::typeIdOfName(name));
		return id;
	}


	int O3dTextInputStream
	::readThingType() {
		return readDictionaryWord(DE_THING_TYPE);
	}


	void O3dTextInputStream
	::readShortArray(ShortArray& dest, int size) {
		unsigned short* array = new unsigned short[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void O3dTextInputStream
	::readByteArray(ByteArray& dest, int size) {
		unsigned char* array = new unsigned char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	void O3dTextInputStream
	::readCharArray(String& dest, int size) {
		char* array = new char[ size ];
		for(int i = 0; i < size; ++i) {
			array[i] = readInt();
		}
		dest.set(array);
	}


	bool O3dTextInputStream
	::eof() {
		return bufferIndex_ == contentsLength_;
	}

}

