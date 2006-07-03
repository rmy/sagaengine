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


#include "CoorT.hpp"

namespace se_core {

#ifdef SE_FIXED_POINT
	scale_t CoorT
	::heightScaler(coor_t dy) {
		// DY as scale
		scale_t min = (static_cast<scale_t>(dy) << SCALE_SHIFT >> COOR_SHIFT);
		// max + 1
		scale_t max = min + SCALE_RES;
		// v = min^2 + SCALE_RES ^ 2
		scale_t v = (min * min) + (static_cast<scale_t>(1) << SCALE_SHIFT * 2);
		// Until max and min are equal
		while(max > min + 1) {
			// Calc middle
			scale_t middle = (max + min) >> 1;
			// middle ^ 2 < v ?
			if(middle * middle < v)
				min = middle;
			else
				max = middle;
		}
		return (static_cast<scale_t>(1) << (SCALE_SHIFT * 2)) / max;
	}

#else

	scale_t CoorT
	::heightScaler(coor_t dy) {
		// DY as scale
		scale_t min = dy;
		// max + 1
		scale_t max = min + SCALE_RES;
		// v = min^2 + SCALE_RES ^ 2
		scale_t v = (min * min) + 1.0f;
		// Until max and min are (nearly) equal
		while(max > min + .001) {
			// Calc middle
			scale_t middle = (max + min) / 2.0f;
			// middle ^ 2 < v ?
			if(middle * middle < v)
				min = middle;
			else
				max = middle;
		}
		return 1.0f / max;
	}

#endif

}
