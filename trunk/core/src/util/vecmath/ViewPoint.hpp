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


#ifndef sim_stat_ViewPoint_hpp
#define sim_stat_ViewPoint_hpp

#include "util/vecmath/Point3.hpp"
#include "util/vecmath/Euler3.hpp"
#include "util/vecmath/Quat4.hpp"

namespace se_core {

	class ViewPoint {
	public:
		ViewPoint() {}
		ViewPoint(const ViewPoint& vp) 
			: face_(vp.face_), coor_(vp.coor_) {
		}

		bool viewPointEquals(const ViewPoint& c) const {
			return (c.coor_.equals(coor_) && c.face_.equals(face_));
		}

		bool viewPointEquals(const ViewPoint& c
							 , coor_t coorEpsilon, bray_t faceEpsilon) const {
			return (c.coor_.epsilonEquals(coor_, coorEpsilon) 
					&& c.face_.epsilonEquals(face_, faceEpsilon));
		}

		inline void setViewPoint(const ViewPoint& original) {
			coor_.set(original.coor_);
			face_.set(original.face_);
		}


		inline void setIdentity() {
			coor_.reset();
			face_.setIdentity();
		}

		#ifdef SE_QUAT
		const Quat4& face() const { return face_; }
		Quat4& face() { return face_; }
		#else
		const Euler3& face() const { return face_; }
		Euler3& face() { return face_; }
		#endif

		inline void setCoor(const Point3& c) { coor_.set(c); }

		inline void setFace(const Quat4& f) { face_.set(f); }
		inline void setFace(const Euler3& f) { face_.set(f); }

		void face(Quat4& dest) {
			dest.set(face_);
		}

		const Point3& coor() const { return coor_; }
		Point3& coor() { return coor_; }

		void interpolate(ViewPoint& vp, scale_t alpha) {
			coor_.interpolate(vp.coor_, alpha);
			face_.interpolate(vp.face_, alpha);
		}


		void add(const ViewPoint& other) {
			coor_.rotate(other.face_);
			coor_.add(other.coor_);
			face_.rotate(other.face_);
		}

		void sub(const ViewPoint& other) {
			face_.rotateInverse(other.face_);
			coor_.sub(other.coor_);
			coor_.rotateInverse(other.face_);
		}

		/**
		 * Set the new face direction.
		 * The new face direction is cached in a special variable that will
		 * be used starting next step.
		 *
		 * @param d The new face direction
		 */
		inline void setFaceDirection(bray_t d) { face_.setYaw(d & BRAY_MASK); }
		const char* toLog() const;

	public: // Attributes
		#ifdef SE_QUAT
		Quat4 face_;
		#else
		Euler3 face_;
		#endif
		Point3 coor_;
	};

}

#endif
