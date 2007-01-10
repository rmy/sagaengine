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


// Don't include this directly. Instead include Trig.hpp



#ifndef base_math_Trig_FloatingPoint_hpp
#define base_math_Trig_FloatingPoint_hpp

#include "util_math.hpp"
#include "util/type/util_type.hpp"
#include "BrayT.hpp"
#include <cmath>

#ifndef SE_FIXED_POINT

namespace se_core {
	class _SeCoreExport Trig {
	public:
		inline static float trigToFloat(trig_t value) {
			return value;
		}

		static quat_t cosQuat(bray_t bray) {
			return ::cos(BrayT::toRad(bray));
		}

		static quat_t sinQuat(bray_t bray) {
			return ::sin(BrayT::toRad(bray));
		}

		inline static coor_t cosScale(coor_t len, bray_t bray) {
			return ::cos(BrayT::toRad(bray)) * len;
		}

		inline static coor_t sinScale(coor_t len, bray_t bray) {
			return ::sin(BrayT::toRad(bray)) * len;
		}

		inline static trig_t tan(bray_t bray) {
			return ::tan(BrayT::toRad(bray));
		}


		inline static bray_t asin(trig_t t) {
			return BrayT::fromRad(::asin(t));
		}


		inline static bray_t acos(trig_t t) {
			return BrayT::fromRad(::acos(t));
		}


		inline static bray_t atan(trig_t t) {
			return BrayT::fromRad(::atan(t));
		}


		inline static bray_t atan2(coor_t xp, coor_t yp) {
			if(yp != 0) {
				//Calculate angle of vector
				bray_t angle=0;
				if (xp!=0) {
					if((xp >= 0) && (yp >= 0))
						angle = atan(yp / xp);
					else if((xp < 0) && (yp >= 0))
						angle = BrayT::DEG180 - atan(yp / -xp);
					else if((xp < 0) && (yp < 0))
						angle = BrayT::DEG180 + atan(-yp / -xp);
					else if((xp >= 0) && (yp < 0))
						angle = BrayT::DEG360 - atan(-yp / xp);
				}
				else {
					if(yp >= 0)
						angle = BrayT::DEG90;
					else
						angle = BrayT::DEG270;
				}
				return angle;
			}
			else {
				if(xp == 0 && yp == 0) return BrayT::DEG0;
				if(xp == 0 && yp > 0) return BrayT::DEG90;
				if(xp == 0 && yp < 0) return BrayT::DEG270;
				if(yp == 0 && xp > 0) return BrayT::DEG0;
				else return BrayT::DEG180;
			}
		}

		static bray_t clockwise8Direction(coor_t xp, coor_t yp);
	};

}

#endif // SE_FIXED_POINT

#endif


