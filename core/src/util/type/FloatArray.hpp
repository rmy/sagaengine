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


#ifndef FloatArray_hpp
#define FloatArray_hpp

namespace se_core {
	/**
	 * Simple float array container.
	 * Contains a array of floats and stores a flag which
	 * signifies wether the contained array should be destroyed when
	 * a new value is set or the FloatArray object itself is destroyed.
	 *
	 * The destruction flag is useful when mixing hardcoded constant
	 * arrays, arrays located in ROM and dynamically created arrays
	 * for the same purpose.
	 */
	class FloatArray {
	public:
		FloatArray();
		~FloatArray();
		void set(float* data, bool doDestroy);
		void set(float* data);
		void set(const float* data);
		const float* get() { return data_; }

	private:
		const float* data_;
		bool doDestroy_;
	};

}

#endif
