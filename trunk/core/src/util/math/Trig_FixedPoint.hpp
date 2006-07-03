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


#ifndef base_math_Trig_FixedPoint_hpp
#define base_math_Trig_FixedPoint_hpp

#include "util_math.hpp"
#include "util/type/util_type.hpp"
#include "BrayT.hpp"

#ifdef SE_FIXED_POINT


namespace se_core {
	class Trig {
	public:
		static const short TRIG_SHIFT = 13;

		inline static float trigToFloat(trig_t value) {
			return value / static_cast<float>(1L << TRIG_SHIFT);
		}

		inline static quat_t trigToQuat(trig_t value) {
			return value << (QUAT_SHIFT - TRIG_SHIFT);
			//return value / static_cast<float>(1L << TRIG_SHIFT);
		}

		static quat_t cosQuat(bray_t bray) {
			return trigToQuat(cosAbs(bray)) * cosSign(bray);
		}

		static quat_t sinQuat(bray_t bray) {
			return trigToQuat(sinAbs(bray)) * sinSign(bray);
		}

		inline static trig_t cosScale(coor_t scale, bray_t bray) {
			return static_cast<coor_t>(((scale * Trig::cosAbs(bray)) >> Trig::TRIG_SHIFT) * Trig::cosSign(bray));
		}

		inline static trig_t sinScale(coor_t scale, bray_t bray) {
			return static_cast<coor_t>(((scale * Trig::sinAbs(bray)) >> Trig::TRIG_SHIFT) * Trig::sinSign(bray));
		}

		inline static trig_t tan(bray_t bray) {
			return tangents[ bray >> BRAY_SHIFT ];
		}

		inline static bray_t asin(trig_t t) {
			//TODO:
			return 0;
		}
		inline static bray_t acos(trig_t t) {
			//TODO:
			return 0;
		}


		/**
		* t should be shifted left TRIG_SHIFT...
		*/
		inline static bray_t atan(trig_t t) {
			bray_t max, min;
			if(t >= 0) {
				max = BrayT::DEG90;
				min = 0;
			}
			else {
				min = BrayT::DEG90 + 1;
				max = BRAY_RANGE;
			}

			while(min + 1 < max) {
				int middle = (min + max) / 2;
				if(t > tangents[middle])
					min = middle;
				else
					max = middle;
			}
			return min;
		}


		inline static bray_t atan2(int xp, int yp) {
			if(yp != 0) {
				//Calculate angle of vector
				short angle=0;
				if (xp!=0) {
					if((xp >= 0) && (yp >= 0))
						angle = atan((yp << 9) / xp);
					else if((xp < 0) && (yp >= 0))
						angle = BrayT::DEG180 - atan((yp << 9) / -xp);
					else if((xp < 0) && (yp < 0))
						angle = BrayT::DEG180 + atan(-(yp << 9) / -xp);
					else if((xp >= 0) && (yp < 0))
						angle = BrayT::DEG360 - atan(-(yp << 9) / xp);
				}
				else {
					if(yp >= 0)
						angle = BrayT::DEG180;
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

	private:
		inline static trig_t sinAbs(bray_t bray) {
			return sinesAbs[ bray >> BRAY_SHIFT ];
		}


		inline static trig_sign_t sinSign(bray_t bray) {
			return sinesSign[ bray >> BRAY_SHIFT ];
		}


		inline static trig_sign_t cosAbs(bray_t bray) {
			return cosinesAbs[ bray >> BRAY_SHIFT ];
		}


		inline static trig_sign_t cosSign(bray_t bray) {
			return cosinesSign[ bray >> BRAY_SHIFT ];
		}

		static const trig_t cosinesAbs[];
		static const trig_t cosinesSign[];
		static const trig_t sinesAbs[];
		static const trig_t sinesSign[];
		static const trig_t tangents[];
	};

}

#endif // SE_FIXED_POINT

#endif



