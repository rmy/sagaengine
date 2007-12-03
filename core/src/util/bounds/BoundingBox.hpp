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


#ifndef util_bounds_BoundingBox_hpp
#define util_bounds_BoundingBox_hpp

#include "../type/util_type.hpp"
#include "../vecmath/Point3.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	class _SeCoreExport BoundingBox {
	public:
		BoundingBox() : minX_(0), minY_(0), minZ_(0), maxX_(0), maxY_(0), maxZ_(0) {}

		BoundingBox(const Point3& p, coor_t radius)
			: minX_(p.x_ - radius), minY_(p.y_ - radius), minZ_(p.z_ - radius)
			, maxX_(p.x_ + radius), maxY_(p.y_ + radius), maxZ_(p.z_ + radius) {
		}

		BoundingBox(const Point3& p, coor_t radius, coor_t height)
			: minX_(p.x_ - radius), minY_(p.y_), minZ_(p.z_ - radius)
			, maxX_(p.x_ + radius), maxY_(p.y_ + height), maxZ_(p.z_ + radius) {
		}

		BoundingBox(const Point3& p, const BoundingBox& b)
			: minX_(p.x_ + b.minX_), minY_(p.y_ + b.minY_), minZ_(p.z_ + b.minZ_)
			, maxX_(p.x_ + b.maxX_), maxY_(p.y_ + b.maxY_), maxZ_(p.z_ + b.maxZ_) {
		}

		inline void testIntegrity() const {
			Assert(maxX_ >= minX_);
			Assert(maxY_ >= minY_);
			Assert(maxZ_ >= minZ_);
		}

		void reset() {
			minX_ = minY_ = minZ_ = 0;
			maxX_ = maxY_ = maxZ_ = 0;
		}

		inline void setMin(coor_t x, coor_t y, coor_t z) {
			minX_ = x;
			minY_ = y;
			minZ_ = z;
		}


		inline void setMax(coor_t x, coor_t y, coor_t z) {
			maxX_ = x;
			maxY_ = y;
			maxZ_ = z;
		}


		bool isNull() const {
			return (minX_ == maxX_ && minY_ == maxY_ && minZ_ == maxZ_);
		}


		void merge(BoundingBox &b) {
			/*
			if(isNull()) {
				*this = b;
				return;
			}
			*/

			if(b.minX_ < minX_) minX_ = b.minX_;
			if(b.minY_ < minY_) minY_ = b.minY_;
			if(b.minZ_ < minZ_) minZ_ = b.minZ_;

			if(b.maxX_ > maxX_) maxX_ = b.maxX_;
			if(b.maxY_ > maxY_) maxY_ = b.maxY_;
			if(b.maxZ_ > maxZ_) maxZ_ = b.maxZ_;
		}


		bool hasInside(coor_t x, coor_t y, coor_t z) const {
			return (x >= minX_ && x < maxX_
					&& y >= minY_ && y < maxY_
					&& z >= minZ_ && z < maxZ_);
		}


		bool hasInside(const Point3& p) const {
			return (p.x_ >= minX_ && p.x_ < maxX_
					&& p.y_ >= minY_ && p.y_ < maxY_
					&& p.z_ >= minZ_ && p.z_ < maxZ_);
		}

		bool isTouching(const Point3& p) const {
			testIntegrity();
			return (p.x_ >= minX_ && p.x_ <= maxX_
					&& p.y_ >= minY_ && p.y_ <= maxY_
					&& p.z_ >= minZ_ && p.z_ <= maxZ_);
		}


		bool isTouching(const Point3& p, coor_t epsilon) const {
			testIntegrity();
			return (p.x_ >= minX_ - epsilon && p.x_ <= maxX_ + epsilon
					&& p.y_ >= minY_ - epsilon && p.y_ <= maxY_ + epsilon
					&& p.z_ >= minZ_ - epsilon && p.z_ <= maxZ_ + epsilon);
		}



		bool isTouchingXZ(const Point3& p) const {
			testIntegrity();
			return (p.x_ >= minX_ && p.x_ <= maxX_
					&& p.z_ >= minZ_ && p.z_ <= maxZ_);
		}


		bool hasInside(const Point3& offset, const Point3& p) const {
			testIntegrity();
			return (p.x_ >= offset.x_ + minX_ && p.x_ < offset.x_ + maxX_
					&& p.y_ >= offset.y_ + minY_ && p.y_ < offset.y_ + maxY_
					&& p.z_ >= offset.z_ + minZ_ && p.z_ < offset.z_ + maxZ_);
		}


		void center(Point3& out) const {
			out.x_ = CoorT::half(minX_ + maxX_);
			out.y_ = CoorT::half(minY_ + maxY_);
			out.z_ = CoorT::half(minZ_ + maxZ_);
		}


		bool isTouching(const BoundingBox& b) const {
			testIntegrity();
			if(b.maxX_ < minX_
				   || b.minX_ > maxX_
				   || maxX_ < b.minX_
				   || minX_ > b.maxX_)
				return false;

			if(b.maxY_ < minY_
				   || b.minY_ > maxY_
				   || maxY_ < b.minY_
				   || minY_ > b.maxY_)
				return false;

			if(b.maxZ_ < minZ_
				   || b.minZ_ > maxZ_
				   || maxZ_ < b.minZ_
				   || minZ_ > b.maxZ_)
				return false;
			return true;
		}

		coor_t radius() const {
			return (maxX_ - minX_ > maxZ_ - minZ_) ? CoorT::half(maxX_ - minX_) : CoorT::half(maxZ_ - minZ_);
		}

		coor_t smallRadius() const {
			return (maxX_ - minX_ < maxZ_ - minZ_) ? CoorT::half(maxX_ - minX_) : CoorT::half(maxZ_ - minZ_);
		}

		coor_t minX_, minY_, minZ_;
		coor_t maxX_, maxY_, maxZ_;
	};

	se_err::Log& operator<< (se_err::Log& log, const BoundingBox& b);

}

#endif
