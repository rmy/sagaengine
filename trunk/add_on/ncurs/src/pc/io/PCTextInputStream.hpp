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


#ifndef pc_io_PCTextInputStream_hpp
#define pc_io_PCTextInputStream_hpp

#include "io/stream/TextInputStream.hpp"
#include "pc_io.hpp"
#include "PCFile.hpp"

namespace se_ncurs {
	class PCTextInputStream : public se_core::TextInputStream {
	public:
		PCTextInputStream();
		PCTextInputStream(const char* directory, const char* filename);
		virtual ~PCTextInputStream();

		int readInt();
		unsigned int readHeaderCode();
		unsigned short readLanguageCode();
		float readFloat();
		void readString(char* dest, short maxLen);
		void readString(se_core::String& dest);
		void readLine(char* dest, short maxLen);
		void readLine(se_core::String& dest);

		int readInfoCode();
		int readPhraseType();
		int readThingType();
		void readShortArray(se_core::ShortArray& dest, int size);
		void readByteArray(se_core::ByteArray& dest, int size);
		void readCharArray(se_core::String& dest, int size);
		bool eof();
		const char* name() { return file_.filename(); }

	private:
		void nextToken();
		char* readString();
		char buffer_[256];

		friend class PCFileManager;
		PCFile file_;
	};
}

#endif
