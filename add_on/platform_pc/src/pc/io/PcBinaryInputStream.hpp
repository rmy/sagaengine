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


#ifndef pc_io_PcBinaryInputStream_hpp
#define pc_io_PcBinaryInputStream_hpp

#include "io/stream/BinaryInputStream.hpp"
#include "pc_io.hpp"
#include "PcFile.hpp"
#include "util/type/util_type.hpp"

namespace se_pc {
	class _SePcExport PcBinaryInputStream : public se_core::BinaryInputStream {
	public:
		PcBinaryInputStream();
		PcBinaryInputStream(const char* directory, const char* filename);
		virtual ~PcBinaryInputStream();

		unsigned int readHeaderCode();
		unsigned short readLanguageCode();
		unsigned short readShort();
		int readInt();
		float readFloat();
		void readLine(char* dest, short maxLen) { readString(dest, maxLen); }
		void readLine(se_core::String& dest) { readString(dest); }
		void readString(char* dest, short maxLen);
		void readString(se_core::String& dest);

		int readInfoCode();
		int readPhraseType();

		void readShortArray(se_core::ShortArray& dest, int size);
		void readByteArray(se_core::ByteArray& dest, int size);
		void readCharArray(se_core::String& dest, int size);
		bool eof();
		const char* name() const { return file_.filename(); }

	private:
		friend class PcFileManager;
		PcFile file_;
	};
}

#endif
