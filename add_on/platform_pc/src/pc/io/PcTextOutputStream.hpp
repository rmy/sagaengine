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


#ifndef pc_io_PcTextOutputStream_hpp
#define pc_io_PcTextOutputStream_hpp

#include "io/stream/OutputStream.hpp"
#include "pc_io.hpp"
#include "PcFile.hpp"

namespace se_pc {
	class _SePcExport PcTextOutputStream : public se_core::OutputStream {
	public:
		PcTextOutputStream();
		PcTextOutputStream(const char* directory, const char* filename);
		virtual ~PcTextOutputStream();

		const char* name() { return sourceFilename_; }
		void writeHeaderCode(unsigned int code);
		void writeInfoCode(int code);
		void writeInt(int value);
		void writeShort(unsigned short value);
		void writeByte(unsigned char value);
		void writeFloat(float value);
		void writeString(const char* string);

	private:
		char sourceFilename_[512];
	};
}

#endif
