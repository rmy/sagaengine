/**
 * Vector math library.
 * The vectore math module is based on the Java3D specification, but coordinates
 * are fixed point decimal rather than floating point. This ensures high performance
 * also on platforms that are not optimized for floating point operations.
 *
 * PS! The Matrix classes are still floating point, so they should not be used in the
 * platform independent module. They are included for feature completeness and for
 * usage in the platform dependent layer for platforms optimised for floating point
 * operations.
 */

#ifndef base_vecmath_hpp
#define base_vecmath_hpp

#include "util/type/util_type.hpp"
namespace se_core {
	class Tuple2;
	class Tuple3;
	class Tuple4;
	class Point2;
	class Point3;
	class Point4;
	class Vector2;
	class Vector3;
	class Vector4;

	class Bounds;

	class AxisAngle4;
	class Euler3;
	class Quat4;

	class ViewPoint;
}


#endif
