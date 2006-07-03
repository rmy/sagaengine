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


#ifndef base_bounds_BoundingSquare_hpp
#define base_bounds_BoundingSquare_hpp

#include "util/type/util_type.hpp"
#include "util/math/CoorT.hpp"

namespace se_core {
	class BoundingSquare {
 	public:
		/**
		 * Constructor for uninitialised BoundingSquare.
		 */
		BoundingSquare() {
		}


		/**
		 * BoundingSquare from BoundingSphere coordinates.
		 */
		BoundingSquare(coor_t x, coor_t y, coor_t radius)
			: minX_(x - radius), minY_(y - radius), maxX_(x + radius), maxY_(y + radius) {
		}


		/**
		 * Set all bounds to 0.
		 */
		inline void setNull() {
			minX_ = 0; maxX_ = 0;
			minY_ = 0; maxY_ = 0;
		}


		/**
		 * @returns true of the bounds have no extension.
		 */
		bool isNull() {
			return (minX_ == maxX_ && minY_ == maxY_);
		}


		inline void setMin(coor_t x, coor_t y) {
			minX_ = x;
			minY_ = y;
		}


		inline void setMax(coor_t x, coor_t y) {
			maxX_ = x;
			maxY_ = y;
		}


		void merge(BoundingSquare &b) {
			if(isNull()) {
				*this = b;
				return;
			}

			if(b.minX_ < minX_) minX_ = b.minX_;
			if(b.minY_ < minY_) minY_ = b.minY_;

			if(b.maxX_ > maxX_) maxX_ = b.maxX_;
			if(b.maxY_ > maxY_) maxY_ = b.maxY_;
		}


		bool hasInside(coor_t x, coor_t y) {
			return (x >= minX_ && x < maxX_ && y >= minY_ && y < maxY_);
		}


		bool isTouching(BoundingSquare& b) {
			bool xInside = ((maxX_ < b.minX_) && (minX_ < b.maxX_))
				|| ((b.maxX_ < minX_) && (b.minX_ < minX_));
			bool yInside = ((maxY_ < b.minY_) && (minY_ < b.maxY_))
				|| ((b.maxY_ < minY_) && (b.minY_ < minY_));
			return (xInside && yInside);
		}


		coor_t radius() {
			return CoorT::half(maxY_ - minY_);
		}


		coor_t centerX() {
			return CoorT::half(minX_ + maxX_);
		}


		coor_t centerY() {
			return CoorT::half(minY_ + maxY_);
		}


	private:
		coor_t minX_, minY_;
		coor_t maxX_, maxY_;
	};
}

#endif
