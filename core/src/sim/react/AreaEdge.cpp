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


#include "AreaEdge.hpp"
#include "CollisionComponent.hpp"
#include "../schema/SimSchema.hpp"
#include "../pos/Pos.hpp"


namespace se_core {

	AreaEdge
	::AreaEdge() : controlPointCount_(0) {
	}


	void AreaEdge
	::addLink(Point2& p1, Point2& p2) {
		int index1 = _getOrAddControlPoint(p1);
		int index2 = _getOrAddControlPoint(p2);
		AssertFatal(links_[ index1 ] == -1, "The point " << p1 << " already links to another point");

		links_[ index1 ] = index2;
	}


	void AreaEdge
	::removeLink(Point2& p1, Point2& p2) {
		int index1 = _getOrAddControlPoint(p1);
		int index2 = _getOrAddControlPoint(p2);
		AssertFatal(links_[ index1 ] == index2, "The point " << p1 << " doesn't link to another point");

		links_[ index1 ] = -1;
	}

	int AreaEdge
	::_getOrAddControlPoint(Point2& p) {
		// Find control point and return the index if it already exists.
		for(int i = 0; i < controlPointCount_; ++i) {
			if(controlPoints_[ i ].equals(p))
				return i;
		}
		// Didn't exist, so add it and return the new index
		Assert(controlPointCount_ < MAX_CONTROL_POINTS);
		controlPoints_[ controlPointCount_ ] = p;
		links_[ controlPointCount_ ] = -1;
		return controlPointCount_++;
	}


	coor_t AreaEdge
	::distance(Point2& p, Point2& dest) const {
		Point2 tmp;
		coor_double_t nearest = 65536.0f * 65536.0f, d;
		for(int i = 0; i < controlPointCount_; ++i) {
			// Is there a link from this control point?
			if(links_[ i ] == -1) {
				// No? Try next then.
				continue;
			}

			// Calculate nearest point on this line
			tmp.nearestPoint(controlPoints_[i], controlPoints_[ links_[ i ] ], p);

			// Closer than nearest on other lines?
			if(nearest < 0 || (d = tmp.distanceSquared(p)) < nearest) {
				nearest = d;
				dest.set(tmp);
			}
		}
		return CoorT::sqrt(nearest);
	}
}


