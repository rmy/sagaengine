#include "../math/Trig.hpp"
#include "../math/QuatT.hpp"
#include "Quat4.hpp"
#include "Euler3.hpp"
#include "AxisAngle4.hpp"

namespace se_core {
	const Quat4 Quat4::IDENTITY(0, 0, 0, 1);

	void Quat4
	::mul(const Quat4& q1, const Quat4& q2) {
		// store on stack for aliasing-safty
		set(
			QuatT::fmul(q1.x_, q2.w_) + QuatT::fmul(q1.w_, q2.x_) + QuatT::fmul(q1.y_, q2.z_) - QuatT::fmul(q1.z_, q2.y_),
			QuatT::fmul(q1.y_, q2.w_) + QuatT::fmul(q1.w_, q2.y_) + QuatT::fmul(q1.z_, q2.x_) - QuatT::fmul(q1.x_, q2.z_),
			QuatT::fmul(q1.z_, q2.w_) + QuatT::fmul(q1.w_, q2.z_) + QuatT::fmul(q1.x_, q2.y_) - QuatT::fmul(q1.y_, q2.x_),
			QuatT::fmul(q1.w_, q2.w_) - QuatT::fmul(q1.x_, q2.x_) - QuatT::fmul(q1.y_, q2.y_) - QuatT::fmul(q1.z_, q2.z_)
		);
	}


	void Quat4
	::mul(const Quat4& q1) {
		// store on stack for aliasing-safty
		set(
			QuatT::fmul(x_, q1.w_) + QuatT::fmul(w_, q1.x_) + QuatT::fmul(y_, q1.z_) - QuatT::fmul(z_, q1.y_),
			QuatT::fmul(y_, q1.w_) + QuatT::fmul(w_, q1.y_) + QuatT::fmul(z_, q1.x_) - QuatT::fmul(x_, q1.z_),
			QuatT::fmul(z_, q1.w_) + QuatT::fmul(w_, q1.z_) + QuatT::fmul(x_, q1.y_) - QuatT::fmul(y_, q1.x_),
			QuatT::fmul(w_, q1.w_) - QuatT::fmul(x_, q1.x_) - QuatT::fmul(y_, q1.y_) - QuatT::fmul(z_, q1.z_)
		);
	}


	void Quat4
	::mulInverse(const Quat4& q1, const Quat4& q2) {
		// zero-div may occur.
		scale_t n = QuatT::oneOver( norm() );

		// store on stack once for aliasing-safty
		set(
				QuatT::scale(n, 
						(QuatT::fmul(q1.x_, q2.w_)
								- QuatT::fmul(q1.w_, q2.x_)
								- QuatT::fmul(q1.y_, q2.z_)
								+ QuatT::fmul(q1.z_, q2.y_))
						),
				QuatT::scale(n, 
						(QuatT::fmul(q1.y_, q2.w_)
								- QuatT::fmul(q1.w_, q2.y_)
								- QuatT::fmul(q1.z_, q2.x_)
								+ QuatT::fmul(q1.x_, q2.z_))),
				QuatT::scale(n, 
						(QuatT::fmul(q1.z_, q2.w_)
								- QuatT::fmul(q1.w_, q2.z_)
								- QuatT::fmul(q1.x_, q2.y_)
								+ QuatT::fmul(q1.y_, q2.x_))),
				QuatT::scale(n, 
						(QuatT::fmul(q1.w_, q2.w_)
								+ QuatT::fmul(q1.x_, q2.x_)
								+ QuatT::fmul(q1.y_, q2.y_)
								+ QuatT::fmul(q1.z_, q2.z_)))
				);
	}


	void Quat4
	::mulInverse(const Quat4& q1) {
		// zero-div may occur.
		scale_t n = QuatT::oneOver( norm() );

		// store on stack once for aliasing-safty
		set(
				QuatT::scale(n, 
						(QuatT::fmul(x_, q1.w_)
								- QuatT::fmul(w_, q1.x_)
								- QuatT::fmul(y_, q1.z_)
								+ QuatT::fmul(z_, q1.y_))
						),
				QuatT::scale(n, 
						(QuatT::fmul(y_, q1.w_)
								- QuatT::fmul(w_, q1.y_)
								- QuatT::fmul(z_, q1.x_)
								+ QuatT::fmul(x_, q1.z_))),
				QuatT::scale(n, 
						(QuatT::fmul(z_, q1.w_)
								- QuatT::fmul(w_, q1.z_)
								- QuatT::fmul(x_, q1.y_)
								+ QuatT::fmul(y_, q1.x_))),
				QuatT::scale(n, 
						(QuatT::fmul(w_, q1.w_)
								+ QuatT::fmul(x_, q1.x_)
								+ QuatT::fmul(y_, q1.y_)
								+ QuatT::fmul(z_, q1.z_)))
				);

	}


	void Quat4
	::set(const AxisAngle4& a1) {
		x_ = a1.x_;
		y_ = a1.y_;
		z_ = a1.z_;

		coor_t n = QuatT::oneOver( CoorT::sqrt( CoorT::pow2(x_) +  CoorT::pow2(y_) +  CoorT::pow2(z_)) );
		//LogMsg(toLog() << ":n " << n);

		#ifdef SE_FIXED_POINT
		#error "Not implemented"
		#endif

		// zero-div may occur.
		//coor_t s = ::sin(0.5*a1.angle_)/n;
		coor_t s = Trig::sinScale(n, a1.angle_ >> 1);
		//LogMsg(toLog() << ":s " << s);

		x_ *= s;
		y_ *= s;
		z_ *= s;
		w_ = Trig::cosQuat(a1.angle_ >> 1);
		//LogMsg(toLog());
	}

	void Quat4
	::setEuler(const Euler3& a1) {
		//LogMsg(a1.toLog());

		/*
		Quat4 q;
		q.set(AxisAngle4(1, a1));
		setEuler(a1.yaw_, a1.pitch_);
		mul(q);
		*/

		// Assuming the angles are in braybrookians (bray_t).
		trig_t c1 = Trig::cosQuat(a1.yaw_ >> 1);
		trig_t c2 = Trig::cosQuat(a1.roll_ >> 1);

		trig_t c3 = Trig::cosQuat(a1.pitch_ >> 1);
		trig_t s1 = Trig::sinQuat(a1.yaw_ >> 1);
		trig_t s2 = Trig::sinQuat(a1.roll_ >> 1);
		trig_t s3 = Trig::sinQuat(a1.pitch_ >> 1);
		// TODO: fixed point mul
		#ifdef SE_FIXED_POINT
		LogFatal("Not implemented");
		#endif
		trig_t c1c2 = c1*c2;
		trig_t s1s2 = s1*s2;
		w_ = c1c2*c3 - s1s2*s3;
		x_ = c1c2*s3 + s1s2*c3;
		y_ = s1*c2*c3 + c1*s2*s3;
		z_ = c1*s2*c3 - s1*c2*s3;

		normalize();

		
		//LogMsg(Euler3(*this).toLog());
	}

	void Quat4
	::setEuler(const bray_t yaw, const bray_t pitch, const bray_t roll) {
		setEuler(Euler3(yaw, pitch, roll));
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
		w_ = c1*c3;
		x_ = c1*s3;
		y_ = s1*c3;
		z_ = -s1*s3;

		normalize();
	}


	void Quat4
	::setPitch(const bray_t pitch) {
	    Assert(pitch == (pitch & BRAY_MASK));

		// Assuming the angles are in braybrookians (bray_t).
		w_ = Trig::cosQuat(pitch >> 1);
		x_ = Trig::sinQuat(pitch >> 1);
		y_ = 0;
		z_ = 0;

		normalize();
	}


	void Quat4
	::setRoll(const bray_t roll) {
		w_ = Trig::cosQuat(roll >> 1);
		x_ = 0;
		y_ = 0;
		z_ = Trig::sinQuat(roll >> 1);

		normalize();
	}

	void Quat4
	::setEuler(const bray_t yaw) {
		// Assuming the angles are in braybrookians (bray_t).
	    Assert(yaw == (yaw & BRAY_MASK));
		
		w_ = Trig::cosQuat(yaw >> 1);
		x_ = 0;
		y_ = Trig::sinQuat(yaw >> 1);
		z_ = 0;
		//LogMsg(yaw << " = (" << QuatT::toFloat(x_) << ", " << QuatT::toFloat(y) << ", " << QuatT::toFloat(z) << ", " << QuatT::toFloat(w) << ")");

		normalize();
	}
	
	void Quat4
	::slerp(const Quat4& q1, scale_t alpha, bool findShortestPath) {
		#ifdef SE_FIXED_POINT
		LogFatal("Not yet implemented");
		#else

		//LogMsg("Alpha " << alpha);
		//LogMsg("this: " << x_ << ", " << y_ << ", " << z_ << ", " << w_);
		//LogMsg("Q1  : " << q1.x_ << ", " << q1.y_ << ", " << q1.z_ << ", " << q1.w_);
		// Both parameters must be pre-normalized
		Assert(isNormalized());
		Assert(q1.isNormalized());
		/*
		// From Hoggar.
		normalize();

		// zero-div may occur.
		scale_t n1 = QuatT::oneOver( CoorT::sqrt( q1.norm() ) );
		coor_t x1 = QuatT::scale(n1, q1.x_);
		coor_t y1 = QuatT::scale(n1, q1.y_);
		coor_t z1 = QuatT::scale(n1, q1.z_);
		coor_t w1 = QuatT::scale(n1, q1.w_);
		*/

		const coor_t x1 = q1.x_;
		const coor_t y1 = q1.y_;
		const coor_t z1 = q1.z_;
		const coor_t w1 = q1.w_;
		float cos_t = x_*x1 + y_*y1 + z_*z1 + w_*w1;

		// t is cosine (dot product)
		// same quaternion (avoid domain error)
		if (1.0 <= CoorT::abs(cos_t))
			return;

		// t is now theta
		float theta = ::acos(cos_t);

		float sin_t = ::sin(theta);

		// same quaternion (avoid zero-div)
		if (sin_t == 0)
			return;

		scale_t s = ::sin((1.0-alpha)* cos_t)/sin_t;
		scale_t t = ::sin(alpha* cos_t)/sin_t;

		if(findShortestPath && t < 0) {
			s = -s;

			x_ = s*x_ + t*x1;
			y_ = s*y_ + t*y1;
			z_ = s*z_ + t*z1;
			w_ = s*w_ + t*w1;

			//normalize();
		}
		else {
			// set values
			x_ = s*x_ + t*x1;
			y_ = s*y_ + t*y1;
			z_ = s*z_ + t*z1;
			w_ = s*w_ + t*w1;
		}
		
		// Stay stable
		normalize();

		#endif
	}


	void Quat4
	::slerp(const Quat4& q1, const Quat4& q2, scale_t alpha, bool findShortestPath) {
		set(q1);
		slerp(q2, alpha, findShortestPath);
	}


	const char* Quat4
	::toLog() const {
		sprintf(log_msg(), "Quat4(%f, %f, %f, %f)", x_, y_, z_, w_);
		return log_msg();
	}

}
