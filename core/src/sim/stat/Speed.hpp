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


#ifndef engine_stat_Speed_hpp
#define engine_stat_Speed_hpp

#include "util/vecmath/Vector3.hpp"

namespace se_core {
  class Speed : public Vector3 {
  public:
	  Speed() : Vector3() {}
	  Speed(coor_t x_, coor_t y_, coor_t z_)
		  : Vector3(x_, y_, z_) {}

	  inline bool isZero() { return ((x_ == 0) && (y_ == 0) && (z_ == 0)); }
	  inline void reset() { x_ = 0; y_ = 0; z_ = 0; }

	  void setSpeedAndDirection(coor_t speed, bray_t bray);
	  void setSpeedAndDirection(coor_t speed, bray_t zxAngle, bray_t heightAngle);
  };
}

#endif
