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


#include "BoundingCylinder.hpp"
#include "BoundingBox.hpp"
#include <cstdio>

namespace se_core {

	bool BoundingCylinder
	::isTouching(const BoundingBox& b) const {
		if(b.maxY_ < minY_
			   || b.minY_ > maxY_
			   || maxY_ < b.minY_
			   || minY_ > b.maxY_)
			return false;

		if(b.maxX_ + radius_ < centerX_
			   || b.minX_ - radius_ > centerX_)
			return false;

		if(b.maxZ_ + radius_ < centerZ_
			   || b.minZ_ - radius_ > centerZ_)
			return false;
		return true;
	}


	BoundingCylinder
	::BoundingCylinder(const Point3& p, const BoundingBox& b)
		: centerX_(p.x_ + CoorT::half(b.minX_ + b.maxX_))
		, centerZ_(p.z_ + CoorT::half(b.minZ_ + b.maxZ_))
		, minY_(p.y_ + b.minY_), maxY_(p.y_ + b.maxY_)
		, radius_(b.smallRadius()) {
	}


	se_err::Log& operator<< (se_err::Log& log, const BoundingCylinder& b) {
		sprintf(log.tmp(), " BoundingCylinder{centre(%f,%f),radius(%f),y(%f,%f)}",
			b.centerX_, b.centerZ_, b.radius_, b.minY_,
			b.maxY_
		);
		log.copy(log.tmp());
		return log;
	}

}

