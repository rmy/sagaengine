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


#ifndef ByteArray_hpp
#define ByteArray_hpp

namespace se_core {
	/**
	 * Simple byte array container.
	 * Contains a array of bytes (unsigned char) and stores a flag which
	 * signifies wether the contained array should be destroyed when
	 * a new value is set or the ByteArray object itself is destroyed.
	 *
	 * The destruction flag is useful when mixing hardcoded constant
	 * arrays, arrays located in ROM and dynamically created arrays
	 * for the same purpose.
	 */
	class _SeCoreExport ByteArray {
	public:
		inline ByteArray() : data_(0), doDestroy_(false) {};
		virtual ~ByteArray();
		void set(const unsigned char* data);
		void set(unsigned char* data);
		const unsigned char* get() { return data_; }

	private:
		const unsigned char* data_;
		bool doDestroy_;
	};

}

#endif
