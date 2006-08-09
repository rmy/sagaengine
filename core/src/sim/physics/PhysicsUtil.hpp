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


#ifndef engine_physics_PhysicsUtil_hpp
#define engine_physics_PhysicsUtil_hpp

#include "util/type/util_type.hpp"
#include "util/math/CoorT.hpp"
#include "util/vecmath/Vector3.hpp"


namespace se_core {
	class PhysicsUtil {
	public:
		/**
		 * Calculate how much speed should be scaled along the
		 * XZ-plane.
		 * @param f a precalculed vector with info about height change
		 *          along the x and z plane per unit vector
		 * @dir The direction that is moved along
		 * @tmp a tmp speed, supplied so that this method (which is called often)
		 *      does not have to create this tmp value on the stack inside loops
		 */
		inline static scale_t heightXZScaler(Vector3& f, bray_t dir, Vector3& tmp) {

			// Calc unit vector (in xz plane) with direction
			tmp.setForward(COOR_RES, dir);

			// Calc height change along unit vector
			coor_t dy = CoorT::abs(CoorT::fromd(CoorT::muld(tmp.x_, f.x_) + CoorT::muld(tmp.z_, f.z_)));
			// Calc how much the speed along the xz-plane
			// as a result of this height difference
			return CoorT::heightScaler(dy);
		}
	private:
	};
}

#endif
