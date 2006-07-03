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


#ifndef InputStream_hpp
#define InputStream_hpp

#include "../../util/type/util_type.hpp"

namespace se_core {
	class InputStream {
	public:
		virtual ~InputStream() {}

		virtual unsigned int readHeaderCode() = 0;
		virtual unsigned short readLanguageCode() = 0;
		virtual int readInt() = 0;
		virtual unsigned short readShort() = 0;
		virtual float readFloat() = 0;

		virtual unsigned short readDictionaryWord(short dictionaryId) = 0;

		virtual void readLine(char* dest, short maxLen) = 0;
		virtual void readLine(String& dest) = 0;
		virtual void readString(char* dest, short maxLen) = 0;
		virtual void readString(String& dest) = 0;

		virtual int readInfoCode() = 0;
		virtual int readPhraseType() = 0;
		virtual int readThingType() = 0;

		virtual void readShortArray(ShortArray& dest, int size) = 0;
		virtual void readByteArray(ByteArray& dest, int size) = 0;
		virtual void readCharArray(String& dest, int size) = 0;

		virtual bool eof() { return true; }

		virtual const char* name() { return "Stream unimplemented"; }
	};
}

#endif
