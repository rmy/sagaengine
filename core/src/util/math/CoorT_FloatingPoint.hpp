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


// Don't include this directly. Instead include CoorT.hpp


#ifndef CoorT_FloatingPoint_hpp
#define CoorT_FloatingPoint_hpp

#include "util_math.hpp"
#include "util/type/util_type.hpp"

#ifndef SE_FIXED_POINT

#ifdef max
#undef max
#endif

namespace se_core {
	class _SeCoreExport CoorT {
	public:
		inline static coor_t half(coor_t t) { return t / 2.0f; }
		inline static coor_t quarter(coor_t t) { return t / 4.0f; }

		inline static coor_t abs(coor_t t) { return t > 0 ? t : -t; }
		inline static coor_double_t absd(coor_double_t t) { return t > 0 ? t : -t; }
		inline static coor_t sign(coor_t t) { return t > 0 ? (coor_t)1 : (coor_t)-1; }
		inline static coor_t signd(coor_double_t t) { return t > 0 ? (coor_double_t)1 : (coor_double_t)-1; }
		inline static coor_t max(coor_t a, coor_t b) { return (a < b) ? b : a; }
		inline static coor_t max(coor_t a, coor_t b, coor_t c) { return max(max(a, b), c); }
		inline static coor_t max(coor_t a, coor_t b, coor_t c, coor_t d) { return max(max(a, b), max(c, d)); }

		static coor_t sqrt(coor_double_t v);

		/**
		 * Calculates the scale that normalizes a vector that is
		 * already normalized in the xz plane, when taking the
		 * y axis into account.
		 */
		static scale_t heightScaler(coor_t dy);

		inline static coor_double_t pow2(coor_double_t v) {
			return v * v;
		}

		inline static coor_t fromd(coor_double_t v) {
			return v;
		}

		inline static coor_t fromScale(coor_double_t v) {
			return v;
		}

		inline static coor_double_t muld(coor_t p1, coor_t p2) {
			return p1 * p2;
		}

		inline static coor_t mul(coor_t p1, coor_t p2) {
			return p1 * p2;
		}


		inline static coor_t div(coor_t p1, coor_t p2) {
			return p1 / p2;
		}

		inline static float toFloat(coor_t value) {
			return value;
		}

		inline static coor_t fromFloat(float value) {
			return value;
		}

		inline static coor_t fromInt(int value) {
			return static_cast<coor_t>(value);
		}

		inline static int toInt(coor_t value) {
			//TODO: Something rotten here
			// (different results for fixed point and floating point)
			return static_cast<int>(value);
		}

		inline static coor_t scale(scale_t s, coor_t c) {
			return s * c;
		}


		inline static coor_tile_t tile(coor_t v) { 
			return static_cast<coor_tile_t>(v); 
		}

		inline static coor_in_t inTile(coor_t v) { 
			return v - static_cast<coor_tile_t>(v); 
		}

		inline static coor_t fromTile(coor_tile_t value) {
			return static_cast<coor_t>(value);
		}

	};

}


#endif // SE_FIXED_POINT

#endif

