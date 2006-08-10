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


#ifndef base_math_FixedPoint_hpp
#define base_math_FixedPoint_hpp

namespace se_core {
	class _SeCoreExport FixedPoint {
	public:
		typedef short value_t;
		typedef int dvalue_t;

		FixedPoint() {}
		FixedPoint(value_t v) : value(v) {}

		static inline value_t fromFloat(const float v) {
			return static_cast<value_t>(v * RESOLUTION);
		}

		static inline value_t fromInt(const int v) {
			return v << SHIFT;
		}

		static inline value_t fromShort(const short v) {
			return v << SHIFT;
		}

		static inline float toFloat(const value_t v) {
			return v / static_cast<float>(RESOLUTION);
		}

		inline operator float() {
			return toFloat(value);
		}

		inline operator int() {
			return value;
		}

		inline operator short() {
			return value;
		}

		inline FixedPoint& operator=(const float& v) {
			value = fromFloat(v);
			return *this;
		}

		inline FixedPoint& operator=(const int& v) {
			value = fromInt(v);
			return *this;
		}

		inline FixedPoint& operator=(const short& v) {
			value = fromShort(v);
			return *this;
		}

		inline FixedPoint& operator=(const FixedPoint& v) {
			value = v.value;
			return *this;
		}

		inline bool operator < (const FixedPoint& v)  const {
			return value < v.value;
		}

		inline bool operator < (const value_t v)  const {
			return value < v;
		}

		inline bool operator < (const int v)  const {
			return value < v;
		}

		inline bool operator > (const FixedPoint& v)  const {
			return value > v.value;
		}

		/*
		inline bool operator > (const value_t v)  const {
			return value > v;
		}
		*/

		inline bool operator == (const FixedPoint& v)  const {
			return value == v.value;
		}

		inline bool operator == (const float& v) const {
			return value == v;
		}

		inline FixedPoint& operator+=(const FixedPoint& v) {
			value += v.value;
			return *this;
		}

		inline FixedPoint& operator-=(const FixedPoint& v) {
			value -= v.value;
			return *this;
		}

		inline FixedPoint& operator*=(const FixedPoint& v) {
			value = (static_cast<dvalue_t>(value) * v.value) >> SHIFT;
			return *this;
		}

		inline FixedPoint& operator*=(const float v) {
			value = static_cast<value_t>(value * v);
			return *this;
		}

		inline FixedPoint& operator*=(const value_t v) {
			//TODO
			return *this;
		}

		inline FixedPoint& operator/=(const FixedPoint& v) {
			value = (static_cast<dvalue_t>(value) << SHIFT) / v.value;
			return *this;
		}

		inline FixedPoint& operator/=(const float v) {
			value = static_cast<value_t>(value / v);
			return *this;
		}

		inline FixedPoint& operator/=(const value_t v) {
			// TODO
			return *this;
		}

		inline FixedPoint operator+(const FixedPoint& v) const {
			return (FixedPoint(*this)).operator+=(v);
		}

		inline FixedPoint operator-(const FixedPoint& v) const {
			return (FixedPoint(*this)).operator-=(v);
		}

		inline FixedPoint operator*(const FixedPoint& v) const {
			return (FixedPoint(*this)).operator*=(v);
		}

		inline FixedPoint operator*(const float& v) const {
			return (FixedPoint(*this)).operator*=(v);
		}

		inline FixedPoint operator*(const value_t v) const {
			return (FixedPoint(*this)).operator*=(v);
		}

		inline FixedPoint operator/(const FixedPoint& v) const {
			return (FixedPoint(*this)).operator/=(v);
		}


		inline FixedPoint operator>>(const value_t v) const {
			return (FixedPoint(*this));
		}


		inline bool operator!() const {
			return value == 0;
		}

		inline FixedPoint operator-() const {
			return FixedPoint(-value);
		}

	private:
		value_t value;
		static const short SHIFT = 8;
		static const short RESOLUTION = 1 << SHIFT;
		static const short MASK = RESOLUTION - 1;
	};



}

#endif
