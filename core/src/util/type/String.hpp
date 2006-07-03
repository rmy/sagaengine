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


#ifndef String_hpp
#define String_hpp

namespace se_core {
	/**
	 * Simple string container.
	 * Contains a C-style string (char array) and stores a flag which
	 * signifies wether the contained string should be destroyed when
	 * a new value is set or the String object itself is destroyed.
	 *
	 * The destruction flag is useful when mixing hardcoded constant
	 * strings, strings located in ROM and dynamically created strings
	 * for the same purpose.
	 */
	class String {
	public:
		inline String() : data_(0), doDestroy_(false) {}
		inline String(const char* data) : data_(data), doDestroy_(false) {}
		inline String(char* data) : data_(data), doDestroy_(true) {}
		~String();
		void set(const char* data);
		void set(char* data);
		inline const char* get() const { return data_; }
		char* copyValue() const;
		bool isEmpty() const;

	private:
		const char* data_;
		bool doDestroy_;
	};

}

#endif
