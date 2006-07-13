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


#ifndef BrayT_hpp
#define BrayT_hpp

#include "util_math.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class BrayT {
	public:
		const static bray_t DEG0 = 0;
		const static bray_t DEG45 = BRAY_RANGE * 1 / 8;
		const static bray_t DEG90 = BRAY_RANGE * 2 / 8;
		const static bray_t DEG135 = BRAY_RANGE * 3 / 8;
		const static bray_t DEG180 = BRAY_RANGE * 4 / 8;
		const static bray_t DEG225 = BRAY_RANGE * 5 / 8;
		const static bray_t DEG270 = BRAY_RANGE * 6 / 8;
		const static bray_t DEG315 = BRAY_RANGE * 7 / 8;
		const static bray_t DEG360 = BRAY_RANGE * 8 / 8;


		inline static bray_t mask(bray_t bray) {
			return bray & BRAY_MASK;
		}

		inline static bray_t add(bray_t b1, bray_t b2) {
			return mask(b1 + b2);
		}

		inline static bray_t sub(bray_t b1, bray_t b2) {
			return mask(b1 - b2);
		}

		inline static float toRad(bray_t bray) {
			return bray * PI * 2 / static_cast<float>(BRAY_RANGE);
		}

		inline static bray_t fromRad(float rad) {
			return static_cast<bray_t>(rad * BRAY_RANGE / (PI * 2));
		}

		inline static float toDeg(bray_t bray) {
			return bray * 360.0f / static_cast<float>(BRAY_RANGE);
		}

		inline static bray_t fromDeg(float deg) {
			return mask(static_cast<bray_t>(deg * BRAY_RANGE / (360.0f)));
		}

		inline static short to4(bray_t bray) {
			static const bray_t mask = (0x0c << (BRAY_RANGE_SHIFT - 4));
			return (bray + DEG45) & mask;
		}

		inline static short to8(bray_t bray) {
			static const bray_t mask = (0x0e << (BRAY_RANGE_SHIFT - 4));
			return (bray + (BRAY_RANGE / 16)) & mask;
		}

		inline static short to16(bray_t bray) {
			static const bray_t mask = (0x0f << (BRAY_RANGE_SHIFT - 4));
			return (bray + (BRAY_RANGE / 32)) & mask;
		}

		inline static short toPlatform(bray_t bray) {
			#ifdef IS_GBA
			return to8(bray);
			#else
			return bray;
			#endif
		}

		inline static short toClockwise4(bray_t bray) {
			return mask((bray + 32) / (BRAY_RANGE >> 2));
		}

		inline static short toClockwise8(bray_t bray) {
			return mask((bray + 16) / (BRAY_RANGE >> 3));
		}

		inline static short toClockwise16(bray_t bray) {
			return mask((bray + 8) / (BRAY_RANGE >> 4));
		}

		inline static short toClockwisePlatform(bray_t bray) {
			#ifdef IS_GBA
			return toClockwise8(bray);
			#else
			return bray;
			#endif
		}

		inline static bray_t fromClockwise4(short dir) {
			return mask(dir << 6);
		}

		inline static bray_t fromClockwise8(short dir) {
			return mask(dir << 5);
		}

		inline static bray_t fromClockwise16(short dir) {
			return mask(dir << 4);
		}

		inline static bray_t fromClockwisePlatform(short dir) {
			#ifdef IS_GBA
			return fromClockwise8(dir);
			#else
			return dir;
			#endif
		}

		/**
		 * Translate to value in range 0 to (BRAY_RANGE / 2),
		 * which equals 0 to 180 degrees.
		 */
		inline static bray_t abs(bray_t v) {
			bray_t m = mask(v);
			return (m >= DEG180) ? (DEG360 - m) : m;
		}

		inline static scale_t abss(scale_t t) { return t > 0 ? t : -t; }
		inline static bray_t fromScale(scale_t v) {
			//return static_cast<bray_t>(abss(v)) & BRAY_MASK;
			return static_cast<bray_t>(v) & BRAY_MASK;
		}

		inline static bray_t scale(scale_t s, bray_t b) {
			return fromScale(s * b);
		}

		inline static bray_t scaleNegative(scale_t s, bray_t b) {
			if(mask(b) < DEG180) {
				return fromScale(s * b);
			}
			else {
				return negate(fromScale(s * negate(mask(b))));
			}
		}

		inline static bray_t invert(bray_t b) {
			return mask(b + DEG180);
		}

		inline static bray_t negate(bray_t b) {
			return (DEG360 - b);
		}

	};

}

#endif
