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


#ifndef base_type_TmpString_hpp
#define base_type_TmpString_hpp

namespace se_core {
	/**
	 * Temporary string.
	 * This string type should only be used as local variables
	 * that are destroyed when the method exits. It reuses
	 * string buffers (located on the heap), and is designed
	 * to avoid bloating the call stack with locally created
	 * strings as some platforms (like the GameBoy Advance) may
	 * have a rather small call stack.
	 * @BUG: Not thread safe! If a later created
	 *    TmpString is used after a former
	 *    is destroyed, it will probably cause trouble.
	 */
	class _SeCoreExport TmpString {
	public:
		TmpString();
		~TmpString() { --usedCount_; }
		char* get() { return strings_[index_]; }

	private:
		short index_;

		const static short MAX_STRING_COUNT = 10;
		static char* strings_[MAX_STRING_COUNT];
		static short usedCount_;
		static short createdCount_;
	};
}

#endif
