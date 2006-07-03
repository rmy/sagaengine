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

namespace se_core {
	class BoundingBox {
	public:
		BoundingBox() : minX_(0), minY_(0), minZ_(0), maxX_(0), maxY_(0), maxZ_(0) {}

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


		bool isNull() {
			return (minX_ == maxX_ && minY_ == maxY_ && minZ_ == maxZ_);
		}


		void merge(BoundingBox &b) {
			if(isNull()) {
				*this = b;
				return;
			}

			if(b.minX_ < minX_) minX_ = b.minX_;
			if(b.minY_ < minY_) minY_ = b.minY_;
			if(b.minZ_ < minZ_) minZ_ = b.minZ_;

			if(b.maxX_ > maxX_) maxX_ = b.maxX_;
			if(b.maxY_ > maxY_) maxY_ = b.maxY_;
			if(b.maxZ_ > maxZ_) maxZ_ = b.maxZ_;
		}


		bool hasInside(coor_t x, coor_t y, coor_t z) {
			return (x >= minX_ && x < maxX_
					&& y >= minY_ && y < maxY_
					&& z >= minZ_ && z < maxZ_);
		}

		coor_t radius() {
#ifdef SE_FIXED_POINT
			return (maxY_ - minY_) >> 1;
#else
			return (maxY_ - minY_) / 2;
#endif
		}

		coor_t minX_, minY_, minZ_;
		coor_t maxX_, maxY_, maxZ_;
	};
}

#endif
