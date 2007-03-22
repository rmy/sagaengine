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


#ifndef pc_io_PcFile_hpp
#define pc_io_PcFile_hpp

#include "util/type/util_type.hpp"
#include <cstdio>

namespace se_pc {
	class _SePcExport PcFile {
	public:
		PcFile();
		PcFile(const char* directory, const char* filename);
		virtual ~PcFile();
		void open(const char* directory, const char* filename);
		void close(void);

		bool eof();

		void readLine(se_core::String& dest);
		void readLine(char* dest, short maxLen);
		void readString(se_core::String& dest);
		short readString(char* dest, short maxLen);

		float readFloat();
		unsigned long readLong();
		unsigned short readShort();
		unsigned char readByte();
		unsigned char nextByte();
		void readShortArray(se_core::ShortArray& dest, int size);
		void readByteArray(se_core::ByteArray& dest, int size);
		void readCharArray(se_core::String& dest, int size);

		long size() const;
		int shortCount() const;
		const char* filename() const;

	private:
		char fullFilePath_[256];
		FILE* in_;
	};
}

#endif
