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

#include "sim_stat.hpp"
#include "Coor.hpp"
#include "SpawnPoint.hpp"
#include "util/vecmath/Point3.hpp"
#include "util/vecmath/Quat4.hpp"
#include "../area/sim_area.hpp"

namespace se_core {

	class ViewPoint {
	public:
		ViewPoint() {}
		ViewPoint(const ViewPoint& vp) : face_(vp.face_), coor_(vp.coor_) {
		}


		bool viewPointEquals(const ViewPoint& c) const {
			return(c.coor_.x_ == coor_.x_
					&& c.coor_.z_ == coor_.z_
					&& c.coor_.y_ == coor_.y_
					&& c.face_.x_ == face_.x_
					&& c.face_.y_ == face_.y_
					&& c.face_.z_ == face_.z_
					&& c.face_.w_ == face_.w_
					);
		}

		inline void setViewPoint(const ViewPoint& original) {
			coor_.set(original.coor_);
			face_.set(original.face_);
		}

		inline void setViewPoint(const SpawnPoint& sp) {
			coor_.set(sp.displace_);
			face_.set(sp.face_);
		}

		const Quat4& face() const { return face_; }
		Quat4& face() { return face_; }

		const Coor& coor() const { return coor_; }
		Coor& coor() { return coor_; }

		/**
		 * Set the new face direction.
		 * The new face direction is cached in a special variable that will be used
		 * starting next step.
		 *
		 * @param d The new face direction
		 */
		inline void setFaceDirection(bray_t d) { face_.setEuler(d & BRAY_MASK); }
		inline void setFace(const Quat4& f) { face_.set(f); }

	public: // Attributes
		Quat4 face_;
		Coor coor_;
	};

}

#endif
