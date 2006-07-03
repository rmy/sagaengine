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


#ifndef Math_hpp
#define Math_hpp

namespace se_core {
	class Math {
	public:
		static short random(short max) {
			static short seed = 1025;
			seed = ~((seed + 254) * 11) / 3;
			return (seed < 0 ? -seed : seed) % max;
		}

		static long sqrt(long v) {
			// Highest max that will prevent overflow
			long max = 0x7fff;
			long min = 0;
			while(max > min + 1) {
				long middle = (max + min) >> 1;
				if(middle * middle < v)
					min = middle;
				else
					max = middle;
			}
			return max;
		}

		/*
		static size_t hashCode(size_t size, const void* ptr) {
			// idea is from Bjarne Stroustrup 3rd $17.6.2.3
			size_t res = 0;
			const char* p = (const char*)ptr;  // reinterpret_cast
			while (size--)
				res = (res << 1) ^ *p++;
			return res;
		}
		*/

	};

}

#endif
