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


#ifndef QuatT_hpp
#define QuatT_hpp

#include "util_math.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport QuatT {
	public:
		inline static quat_t sign(coor_t t) { return t > 0 ? (quat_t)1 : (quat_t)(-1); }
		inline static quat_t abs(quat_t t) { return t > 0 ? t : -t; }
		inline static quat_t max(quat_t a, quat_t b) { return a < b ? b : a; }
		inline static quat_t max(quat_t a, quat_t b, quat_t c) { return max(max(a, b), c); }
		inline static quat_t max(quat_t a, quat_t b, quat_t c, quat_t d) { return max(max(a, b), max(c, d)); }

		static quat_t sqrt(quat_t v) {
			//@TODO: Not implemnted yet
			return 0;
		}

		inline static float toFloat(quat_t value) {
#ifdef SE_FIXED_POINT
			return value / (float)QUAT_RES;
#else
			return value;
#endif
		}

		inline static quat_t fromFloat(float value) {
			return static_cast<quat_t>(value * QUAT_RES);
		}

		inline static quat_t fromCoorT(coor_t value) {
			// quat_t and coor_t should be same basic type
			return value;
		}

		inline static quat_t fmul(quat_t v1, quat_t v2) {
			// quat_t and coor_t should be same basic type
#ifdef SE_FIXED_POINT
			return (v1 >> QUAT_HALF_SHIFT) * (v2 >> QUAT_HALF_SHIFT);
#else
			return v1 * v2;
#endif

		}

		inline static quat_t pow2(quat_t v1) {
			// quat_t and coor_t should be same basic type
#ifdef SE_FIXED_POINT
			quat_t v = (v1 >> QUAT_HALF_SHIFT);
			return v * v;
#else
			return v1 * v1;
#endif

		}


		inline static scale_t oneOver(quat_t v) {
			// quat_t and coor_t should be same basic type
#ifdef SE_FIXED_POINT
			return (QUAT_RES << QUAT_HALF_SHIFT) / (v >> QUAT_HALF_SHIFT);
#else
			return 1 / v;
#endif

		}


		inline static quat_t scale(scale_t s, quat_t c) {
#ifdef SE_FIXED_POINT
			return static_cast<coor_t>(sign(c) * ((s * abs(c)) >> SCALE_SHIFT));
#else
			return s * c;
#endif
		}


	};

}

#endif
