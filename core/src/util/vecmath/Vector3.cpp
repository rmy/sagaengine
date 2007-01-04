#include "Vector3.hpp"
#include "Euler3.hpp"
#include "Quat4.hpp"
#include "../math/BrayT.hpp"
#include "../math/CoorT.hpp"
#include "../math/Trig.hpp"

namespace se_core {
	const Vector3 Vector3::ZERO(0, 0, 0);
	const Vector3 Vector3::UNIT_FORWARD(0, 0, -1);

	coor_t Vector3
	::length() const {
		return CoorT::sqrt(lengthSquared());
	}


	bray_t Vector3
	::angle(const Vector3& v1) const {
		//@TODO: What is this angle???
		// return (double)Math.acos(dot(v1)/v1.length()/v.length());
		// Numerically, near 0 and PI are very bad condition for acos.
		// In 3-space, |atan2(sin,cos)| is much stable.
		Vector3 c;
		c.cross(*this, v1);
		coor_t sin = c.length();

		return BrayT::abs(Trig::atan2(sin, dot(v1)));
	}


	bray_t Vector3
	::yaw() const {
		return Trig::atan2(-z_, x_);
	}


	void Vector3
	::setForward(const coor_t len, const bray_t yaw) {
		x_ = Trig::sinScale(len, yaw);
		z_ = -Trig::cosScale(len, yaw);
	}


	void Vector3
	::setForward(const coor_t len, const bray_t yaw, const bray_t pitch) {
		setForward(len, yaw);

		y_ = -Trig::sinScale(len, pitch);
		x_ = Trig::cosScale(x_, pitch);
		z_ = Trig::cosScale(z_, pitch);
	}


	void Vector3
	::setForward(const coor_t len, const Euler3& a1) {
		setForward(len, a1.yaw_, a1.pitch_);
	}

	/*
	void Vector3
	::toAngle(Angle& dest) {
		// Based on rotation scheme found in Foley page 218-219.

		// Get Yaw
		// Cos to yaw is x divided by length of line projection
		// to yx axis.
		coor_t length = new Vector2(x_, y_).length();
		bray_t yaw;
		if (length > 0)
			yaw = Trig::acos(x_/ length);
		else
			yaw = 0;

		if (y < 0)
			yaw = -yaw;

		// Rotate line so it is directly above x-axis.
		Vector v = rotate(new Angle(0,0,-yaw));

		// Get pitch
		// Cos to pitch is the new lines x divided by the total
		// line length.
		float pitch = (float)Math.acos(v.x / v.getLength());
		if (z<0)
			pitch = -(float)Math.toDegrees(pitch);
		else
			pitch = (float)Math.toDegrees(pitch);

		return new Angle(0,pitch,yaw);
		}
	*/



    void Vector3
	::rotate(const Quat4& q) {
		// nVidia SDK implementation
		Vector3 uv, uuv; 
		Vector3 qvec(q.x_, q.y_, q.z_);
		uv.cross(qvec, *this); 
		uuv.cross(qvec, uv);
		uv.scale(2 * SCALE_RES * q.w_); 
		uuv.scale(2 * SCALE_RES);

		add(uv);
		add(uuv);
    }
	

    void Vector3
	::rotateInverse(const Quat4& q) {
		Quat4 inv(q);
		inv.inverse();
		rotate(inv);
	}


    void Vector3
	::rotate(const Euler3& a) {
		rotate(Quat4(a));
    }

	
    void Vector3
	::rotateInverse(const Euler3& a) {
		rotateInverse(Quat4(a));
		//Euler3 inv(a);
		//inv.invert();
		//rotate(Quat4(inv));
    }
	

    void Vector3
	::rotate(const Vector3& v, const Quat4& q) {
		// nVidia SDK implementation
		Vector3 uv, uuv; 
		Vector3 qvec(q.x_, q.y_, q.z_);
		uv.cross(qvec, v); 
		uuv.cross(qvec, uv);
		uv.scale(2 * SCALE_RES * q.w_); 
		uuv.scale(2 * SCALE_RES);

		set(*this);
		add(uv);
		add(uuv);
    }
	

    void Vector3
	::rotate(const Vector3& v, const Euler3& a) {
		rotate(v, Quat4(a));
	}
}
