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


#ifndef OutputStream_hpp
#define OutputStream_hpp

#include "../../util/type/util_type.hpp"

namespace se_core {
	class OutputStream {
	public:
		virtual ~OutputStream() {}
		virtual void writeHeaderCode(unsigned int code) = 0;
		virtual void writeInt(int value) = 0;
		virtual void writeShort(unsigned short value) = 0;
		virtual void writeByte(unsigned char value) = 0;
		virtual void writeFloat(float value) = 0;
		virtual void writeString(const char* string) = 0;
		virtual void writeSingleValueId(int id) = 0;
		virtual void writeAttributeId(int id) = 0;

		/*
		virtual unsigned short writeLanguageCode() = 0;
		virtual int writeInfoCode() = 0;
		virtual int writeEquipmentSlotId() = 0;
		virtual int writeMovementMode() = 0;
		virtual int writePhraseType() = 0;
		virtual int writeWeaponGroup() = 0;
		virtual int writeWeaponId() { return writeWeaponGroup(); }
		virtual int writeThingType() = 0;

		virtual void writeShortArray(ShortArray& dest, int size) = 0;
		virtual void writeByteArray(ByteArray& dest, int size) = 0;
		virtual void writeCharArray(String& dest, int size) = 0;
		*/
	};
}

#endif
