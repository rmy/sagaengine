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


#include "UtilMath.hpp"
#include "util/error/Log.hpp"
#include "util/math/all.hpp"


using namespace se_core;

namespace test {
	UtilMath
	::UtilMath() {
	}


	UtilMath
	::~UtilMath() {
	}


	void UtilMath
	::run() {
		util_math_FixedPoint();
	}


	void UtilMath
	::util_math_FixedPoint() {

		FixedPoint v1, v2, v3, res1, res2;

		v1 = 0.0f;
		Assert(!v1); // Is zero
		v1 = 2.0f;
		Assert(v1); // Not zero
		v1 = 2.0f;
		v2 = 2.0f;
		v3 = 4.0f;

		Assert(FixedPoint::fromFloat(FixedPoint::toFloat(256)) == 256);

		res1 = v1 * v2;
		res2 = v1 + v2;
		Assert(v1 == v2);
		Assert(res1 == res2);
		Assert(res1 == v3);

		res1 /= v2;
		Assert(res1 == v1);
		res1 = (v1 * v2) / v1;
		Assert(res1 == v2);

		res1 = 0.0f;
		res2 = 0.0f;

		res1 = v1 + v2 + v3;
		res2 += v1;
		res2 += v2;
		res2 += v3;
		Assert(res1 == res2);

		res1 = v1 + v2 + v3;
		res1 -= v2;
		res1 -= v3;
		Assert(res1 == v1);

		float f1 = 2.0f;
		//float f2 = 0.0f;
		v1 = f1;
		//f2 = v1;
		//Assert(f1 == f2);

		LogMsg("UNIT TEST PASSED: base_math_FixedPoint");
	}
}
