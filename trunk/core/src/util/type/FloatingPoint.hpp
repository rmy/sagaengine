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


#ifndef util_type_FloatingPoint_hpp
#define util_type_FloatingPoint_hpp

#ifndef SE_FIXED_POINT

#ifdef SE_DOUBLE
typedef double coor_t;
typedef double coor_double_t;
typedef double trig_t;
#else
typedef float coor_t;
typedef float coor_double_t;
typedef float trig_t;
#endif

typedef int coor_tile_t;
typedef coor_t coor_in_t;
typedef coor_t quat_t;

typedef coor_double_t scale_t;
typedef coor_double_t coor_world_t;

//static const coor_double_t COOR_T_MAX = 0x7fffffff;
//static const coor_double_t COOR_T_MIN = -COOR_T_MAX - 1;

static const coor_t COOR_RES = 1.0f;
static const scale_t SCALE_RES = 1.0f;
static const quat_t QUAT_RES = COOR_RES;

#endif // SE_FIXED_POINT

#endif
