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


#ifndef util_bounds_BoundingCylinder_hpp
#define util_bounds_BoundingCylinder_hpp

#include "../type/util_type.hpp"
#include "../vecmath/Point3.hpp"
#include "util_bounds.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	class _SeCoreExport BoundingCylinder {
	public:
		BoundingCylinder() : centerX_(0), centerZ_(0), minY_(0), maxY_(0), radius_(0) {}

		BoundingCylinder(const Point3& p, coor_t radius)
			: centerX_(p.x_), centerZ_(p.z_)
			, minY_(p.y_ - radius), maxY_(p.y_ + radius)
			, radius_(radius) {
		}

		BoundingCylinder(const Point3& p, coor_t radius, coor_t height)
			: centerX_(p.x_), centerZ_(p.z_)
			, minY_(p.y_), maxY_(p.y_ + height)
			, radius_(radius) {
		}

		BoundingCylinder(const Point3& p, const BoundingCylinder& b)
			: centerX_(p.x_ + b.centerX_), centerZ_(p.z_ + b.centerZ_)
			, minY_(p.y_ + b.minY_), maxY_(p.y_ + b.maxY_)
			, radius_(b.radius_) {
		}

		BoundingCylinder(const Point3& p, const BoundingBox& b);

		void reset() {
			centerX_ = centerZ_ = 0;
			minY_ = maxY_ = 0;
			radius_ = 0;
		}

		bool isNull() const {
			return (radius_ == 0);
		}


		bool hasInside(coor_t x, coor_t y, coor_t z) const {
			if(y < minY_ || y > maxY_)
				return false;

			coor_t xDist = x - centerX_;
			coor_t zDist = z - centerZ_;

			if(xDist * xDist + zDist * zDist > radius_ * radius_)
				return false;

			return true;
		}


		bool hasInside(const Point3& p) const {
			if(p.y_ <= minY_ || p.y_ >= maxY_)
				return false;

			coor_t xDist = p.x_ - centerX_;
			coor_t zDist = p.z_ - centerZ_;

			if(xDist * xDist + zDist * zDist >= radius_ * radius_)
				return false;

			return true;
		}

		bool isTouching(const Point3& p) const {
			if(p.y_ < minY_ || p.y_ > maxY_)
				return false;

			coor_t xDist = p.x_ - centerX_;
			coor_t zDist = p.z_ - centerZ_;

			if(xDist * xDist + zDist * zDist > radius_ * radius_)
				return false;

			return true;
		}


		bool hasInside(const Point3& offset, const Point3& p) const {
			if(offset.y_ + p.y_ <= minY_ || offset.y_ + p.y_ >= maxY_)
				return false;

			coor_t xDist = offset.x_ + p.x_ - centerX_;
			coor_t zDist = offset.z_ + p.z_ - centerZ_;

			if(xDist * xDist + zDist * zDist >= radius_ * radius_)
				return false;

			return true;

		}


		void center(Point3& out) const {
			out.x_ = centerX_;
			out.y_ = CoorT::half(minY_ + maxY_);
			out.z_ = centerZ_;
		}


		bool isTouching(const BoundingCylinder& b) const {
			if(b.maxY_ < minY_
				   || b.minY_ > maxY_
				   || maxY_ < b.minY_
				   || minY_ > b.maxY_)
				return false;

			coor_t radSum = radius_ + b.radius_;
			coor_t distX = centerX_ - b.centerX_;
			coor_t distZ = centerZ_ - b.centerZ_;

			if(radSum * radSum < distX * distX + distZ + distZ)
				return false;

			return true;
		}

		bool isTouching(const BoundingBox& b) const;

		coor_t radius() const {
			return radius_;
		}

		coor_t centerX_, centerZ_;
		coor_t minY_, maxY_;
		coor_t radius_;
	};

	se_err::Log& operator<< (se_err::Log& log, const BoundingCylinder& b);

}

#endif
