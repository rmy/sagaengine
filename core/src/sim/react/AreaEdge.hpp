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


#ifndef engine_react_AreaEdge_hpp
#define engine_react_AreaEdge_hpp

#include "sim_react.hpp"
#include "../thing/sim_thing.hpp"
#include "../physics/sim_physics.hpp"
#include "../pos/sim_pos.hpp"
#include "sim/sim.hpp"
#include "util/vecmath/Point2.hpp"

namespace se_core {

	/** Base reaction class for actor vs thing collisions. */
	class _SeCoreExport AreaEdge {
	public:
		AreaEdge();

		/** 
		 * Add a line that is part of the edge of the area.
		 * Lines should be either clockwise or anti-clockwise memers
		 * of an edge polygon. A mix will most likely fail with an 
		 * assertion, as the same point is likely to link to two other points.
		 */
		void addLink(Point2& p1, Point2& p2);
		void removeLink(Point2& p1, Point2& p2);


		/**
		 * Calculate the distance from the test point to the nearest edge line.
		 */
		coor_t distance(Point2& testPoint, Point2& dest) const;

	private:
		/**
		 * Get the index to the control point if it exists. If not, 
		 * add it and return the new index. This method is for internal use only.
		 */
		int _getOrAddControlPoint(Point2& p);

		/** The number of control points. 
		 * This is also the maximum number of links to other
		 * control points.
		 */
		int controlPointCount_;

		/** The capacity of the controlPoints_ array.
		 */
		const static int MAX_CONTROL_POINTS = 128;

		/** Array containing all registered control points.
		 * The control points are stored in 2D.
		 */
		Point2 controlPoints_[ MAX_CONTROL_POINTS ];

		/** Links (edge line) to other control points.
		 * Each control point only links to next control point in polygon.
		 * Never more than one.
		 */
		int links_[ MAX_CONTROL_POINTS ];
	};
}

#endif
