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
	class _SeCoreExport String {
	public:
		String();
		String(const char* data);
		String(char* data);
		String(const char* data, int toPos);
		~String();
		void set(const char* data);
		void set(char* data);
		void reset();
		void copy(const char* data);
		/**
		 * toPos of zero or negative is from end of string
		 */
		void copy(const char* data, int toPos);
		void copy(const char* data, char toDelim);
		inline const char* get() const { return data_; }
		const char* ext(char delimiter, bool mustFind = true) const;
		char* copyValue() const;
		bool isEmpty() const;
		int len() const;
		void append(const char* s);
		int compare(const String& s) const;
		int compare(const char* s) const;

		bool equals(const String& s) const;
		bool equals(const char* s) const;

	private:
		const char* data_;
		bool doDestroy_;
	};

}

#endif
