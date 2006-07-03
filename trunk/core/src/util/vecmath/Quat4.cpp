#include "../math/Trig.hpp"
#include "../math/QuatT.hpp"
#include "Quat4.hpp"
#include "Euler3.hpp"
//#include <AxisAngle4_.h>

namespace se_core {

	void Quat4
	::mul(const Quat4& q1, const Quat4& q2) {
		// store on stack for aliasing-safty
		set(
				QuatT::fmul(q1.x, q2.w) + QuatT::fmul(q1.w, q2.x) + QuatT::fmul(q1.y, q2.z) - QuatT::fmul(q1.z, q2.y),
				QuatT::fmul(q1.y, q2.w) + QuatT::fmul(q1.w, q2.y) + QuatT::fmul(q1.z, q2.x) - QuatT::fmul(q1.x, q2.z),
				QuatT::fmul(q1.z, q2.w) + QuatT::fmul(q1.w, q2.z) + QuatT::fmul(q1.x, q2.y) - QuatT::fmul(q1.y, q2.x),
				QuatT::fmul(q1.w, q2.w) - QuatT::fmul(q1.x, q2.x) - QuatT::fmul(q1.y, q2.y) - QuatT::fmul(q1.z, q2.z)
				);
	}


	void Quat4
	::mul(const Quat4& q1) {
		// store on stack for aliasing-safty
		set(
				QuatT::fmul(x, q1.w) + QuatT::fmul(w, q1.x) + QuatT::fmul(y, q1.z) - QuatT::fmul(z, q1.y),
				QuatT::fmul(y, q1.w) + QuatT::fmul(w, q1.y) + QuatT::fmul(z, q1.x) - QuatT::fmul(x, q1.z),
				QuatT::fmul(z, q1.w) + QuatT::fmul(w, q1.z) + QuatT::fmul(x, q1.y) - QuatT::fmul(y, q1.x),
				QuatT::fmul(w, q1.w) - QuatT::fmul(x, q1.x) - QuatT::fmul(y, q1.y) - QuatT::fmul(z, q1.z)
				);
	}


	void Quat4
	::mulInverse(const Quat4& q1, const Quat4& q2) {
		// zero-div may occur.
		scale_t n = QuatT::oneOver( norm() );

		// store on stack once for aliasing-safty
		set(
				QuatT::scale(n, 
						(QuatT::fmul(q1.x, q2.w)
								- QuatT::fmul(q1.w, q2.x)
								- QuatT::fmul(q1.y, q2.z)
								+ QuatT::fmul(q1.z, q2.y))
						),
				QuatT::scale(n, 
						(QuatT::fmul(q1.y, q2.w)
								- QuatT::fmul(q1.w, q2.y)
								- QuatT::fmul(q1.z, q2.x)
								+ QuatT::fmul(q1.x, q2.z))),
				QuatT::scale(n, 
						(QuatT::fmul(q1.z, q2.w)
								- QuatT::fmul(q1.w, q2.z)
								- QuatT::fmul(q1.x, q2.y)
								+ QuatT::fmul(q1.y, q2.x))),
				QuatT::scale(n, 
						(QuatT::fmul(q1.w, q2.w)
								+ QuatT::fmul(q1.x, q2.x)
								+ QuatT::fmul(q1.y, q2.y)
								+ QuatT::fmul(q1.z, q2.z)))
				);
	}


	void Quat4
	::mulInverse(const Quat4& q1) {
		// zero-div may occur.
		scale_t n = QuatT::oneOver( norm() );

		// store on stack once for aliasing-safty
		set(
				QuatT::scale(n, 
						(QuatT::fmul(x, q1.w)
								- QuatT::fmul(w, q1.x)
								- QuatT::fmul(y, q1.z)
								+ QuatT::fmul(z, q1.y))
						),
				QuatT::scale(n, 
						(QuatT::fmul(y, q1.w)
								- QuatT::fmul(w, q1.y)
								- QuatT::fmul(z, q1.x)
								+ QuatT::fmul(x, q1.z))),
				QuatT::scale(n, 
						(QuatT::fmul(z, q1.w)
								- QuatT::fmul(w, q1.z)
								- QuatT::fmul(x, q1.y)
								+ QuatT::fmul(y, q1.x))),
				QuatT::scale(n, 
						(QuatT::fmul(w, q1.w)
								+ QuatT::fmul(x, q1.x)
								+ QuatT::fmul(y, q1.y)
								+ QuatT::fmul(z, q1.z)))
				);

	}


	/*
	void Quat4
	::set(const AxisAngle4& a1) {
		x = a1.x;
		y = a1.y;
		z = a1.z;
		coor_t n = CoorT::sqrt(x*x + y*y + z*z);
		// zero-div may occur.
		coor_t s = CoorT::sin(0.5*a1.angle)/n;
		x *= s;
		y *= s;
		z *= s;
		w = CoorT::cos(0.5*a1.angle);
	}
	*/

	void Quat4
	::setEuler(const Euler3& a1) {
		// Assuming the angles are in braybrookians (bray_t).
		trig_t c1 = Trig::cosQuat(a1.yaw_ >> 1);
		trig_t c2 = Trig::cosQuat(a1.roll_ >> 1);
		trig_t c3 = Trig::cosQuat(a1.pitch_ >> 1);
		trig_t s1 = Trig::sinQuat(a1.yaw_ >> 1);
		trig_t s2 = Trig::sinQuat(a1.roll_ >> 1);
		trig_t s3 = Trig::sinQuat(a1.pitch_ >> 1);
		// TODO: fixed point mul
		trig_t c1c2 = c1*c2;
		trig_t s1s2 = s1*s2;
		w = c1c2*c3 - s1s2*s3;
		x = c1c2*s3 + s1s2*c3;
		y = s1*c2*c3 + c1*s2*s3;
		z = c1*s2*c3 - s1*c2*s3;
	}

	void Quat4
	::setEuler(const bray_t yaw, const bray_t pitch) {
	    Assert(yaw == (yaw & BRAY_MASK));
	    Assert(pitch == (pitch & BRAY_MASK));

		// Assuming the angles are in braybrookians (bray_t).
		trig_t c1 = Trig::cosQuat(yaw >> 1);
		trig_t s1 = Trig::sinQuat(yaw >> 1);
		trig_t c3 = Trig::cosQuat(pitch >> 1);
		trig_t s3 = Trig::sinQuat(pitch >> 1);
		// TODO: fixed point mul
		w = c1*c3;
		x = c1*s3;
		y = s1*c3;
		z = -s1*s3;
	}


	void Quat4
	::setEuler(const bray_t yaw) {
		// Assuming the angles are in braybrookians (bray_t).
	    Assert(yaw == (yaw & BRAY_MASK));
		
		w = Trig::cosQuat(yaw >> 1);
		x = 0;
		y = Trig::sinQuat(yaw >> 1);
		z = 0;
		//LogMsg(yaw << " = (" << QuatT::toFloat(x) << ", " << QuatT::toFloat(y) << ", " << QuatT::toFloat(z) << ", " << QuatT::toFloat(w) << ")");
	}
	
	/*
	void Quat4
	::interpolate(scale_t alpha, const Quat4& q1) {
		LogFatal("Not yet implemented");
		// From Hoggar.
		normalize();
		coor_t n1 = CoorT::sqrt(q1.norm());
		// zero-div may occur.
		coor_t x1 = q1.x/n1;
		coor_t y1 = q1.y/n1;
		coor_t z1 = q1.z/n1;
		coor_t w1 = q1.w/n1;

		// t is cosine (dot product)
		coor_t t = x*x1 + y*y1 + z*z1 + w*w1;

		// same quaternion (avoid domain error)
		if (1.0 <= CoorT::abs(t))
			return;

		// t is now theta
		bray_t theta = Trig::acos(t);

		coor_t sin_t = Trig::sinQuat(theta);

		// same quaternion (avoid zero-div)
		if (sin_t == 0)
			return;

		coor_t s = Trig::sinQuat((1.0-alpha)*t)/sin_t;
		t = Trig::sinQuat(alpha*t)/sin_t;

		// set values
		x = s*x + t*x1;
		y = s*y + t*y1;
		z = s*z + t*z1;
		w = s*w + t*w1;
	}

	void Quat4
	::interpolate(scale_t alpha, const Quat4& q1, const Quat4& q2) {
		set(q1);
		interpolate(alpha, q2);
	}
	*/

}
