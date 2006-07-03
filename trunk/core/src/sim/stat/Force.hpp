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


#ifndef Force_hpp
#define Force_hpp

#include "util/vecmath/Vector3.hpp"

namespace se_core {
	class Force : public Vector3 {
	public:
		Force() : Vector3() {}
		Force(coor_t x_, coor_t y_, coor_t z_)
			: Vector3(x_, y_, z_) {}

		inline bool isZero() const { return ((x_ == 0) && (y_ == 0) && (z_ == 0)); }
		inline void reset() { x_ = 0; y_ = 0; z_ = 0; }

		inline void setXInt(coor_t x) { x_ = x; }
		inline void setYInt(coor_t y) { y_ = y; }
		inline void setZInt(coor_t z) { z_ = z; }

		inline coor_t xInt() const { return x_; }
		inline coor_t yInt() const { return y_; }
		inline coor_t zInt() const { return z_; }

		void setSpeedAndDirection(coor_t speed, bray_t angle);
	};
}

#endif
