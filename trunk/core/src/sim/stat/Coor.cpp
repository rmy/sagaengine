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


#include "Coor.hpp"
#include "Pos.hpp"
#include "Force.hpp"
#include "../area/Area.hpp"
#include "../sim.hpp"
#include "../error/sim_error.hpp"
#include "util/math/all.hpp"
#include <cstdio>


namespace se_core {


	coor_t Coor
	::xzDistanceLinf(const Coor& c) const {
		coor_t xDist = CoorT::abs(c.x_ - x_);
		coor_t yDist = CoorT::abs(c.z_ - z_);

		return (xDist > yDist) ? xDist : yDist;
	}


	coor_t Coor
	::yDistance(const Coor& c) const {
		coor_t dist = CoorT::abs(c.y_ - y_);
		return dist;
	}


	bray_t Coor
	::xzAngleTowards(const Point3& c) const {
		coor_t xp = c.x_ - x_;
		coor_t yp = c.z_ - z_;
		return Trig::atan2(-yp, xp);
	}


}
