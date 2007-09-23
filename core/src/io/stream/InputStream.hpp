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
	/**
	 * Input streaming base class.
	 * This class (or rather its subclasses BinaryInputStream and TextInputStream
	 * should be subclassed in a platform dependent layer).
	 */
	class _SeCoreExport InputStream {
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

		virtual const char* name() const = 0;

		int nameLen() const {
			const char* n = name();
			int len = 0;
			for(; n[len] != 0; ++len)
				;
			return len;
		}


		int last(char ch, int start = -1) const {
			// Find end;
			int len = start;
			if(len < 0) {
				len = nameLen() - start;
			}

			// Find first ch from the end
			const char* n = name();
			for(int i = len; i > 0; --i) {
				if(n[i] == ch) {
					return i;
				}
			}
			return -1;
		}


		int basenameLen() const {
			int len = nameLen();
			int p = last('/', len);
			//if(p < 0)
			//	return len;
			//return len - (p + 1);
			// Same but faster. -1 + 1 = 0
			return len - (p + 1);
		}


		const char* basename() const {
			int len = nameLen();
			const char* n = name();
			return &n[ len - basenameLen() ];
		}


		const char* oneDirAndBasename() const {
			int len = nameLen();
			const char* n = name();

			// Find first slash
			int p = last('/', len);
			if(p <= 0)
				return n;

			// Find second slash
			p = last('/', p - 1);
			if(p < 0)
				return n;

			return &n[ p + 1 ];
		}


		int extLen() const {
			int len = nameLen();

			// Find first dot
			int p = last('.', len);

			// No extension?
			if(p < 0)
				return 0;

			return len - p;
		}


		const char* ext() const {
			int len = nameLen();
			const char* n = name();
			return &n[ len - extLen() ];
		}
	};
}

#endif
