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


#ifndef base_type_hpp
#define base_type_hpp

#include "util/config/util_config.hpp"

//#define SE_FIXED_POINT

#ifdef SE_FIXED_POINT
# include "FixedPoint.hpp"
#else
# include "FloatingPoint.hpp"
#endif

typedef unsigned short bray_t;

static const short BRAY_SHIFT = 1;
static const short BRAY_RES = 1 << BRAY_SHIFT;
static const short BRAY_RANGE_SHIFT = 8 + BRAY_SHIFT;
static const short BRAY_RANGE = (1 << BRAY_RANGE_SHIFT);
static const short BRAY_MASK = (BRAY_RANGE - 1);

inline quat_t c2m(coor_t value) { return value; }
inline coor_t m2c(quat_t value) { return value; }


/**
 * Basic SagaEngine types.
 * Datatypes being used by the SagaEngine Core, and
 * optionally by platform dependent modules.
 */
namespace se_core {

	class String;
	class TmpString;
	class ByteArray;
	class ShortArray;
	class FloatArray;
}


#endif
