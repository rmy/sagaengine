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


#ifndef util_type_FixedPoint_hpp
#define util_type_FixedPoint_hpp

#ifdef SE_FIXED_POINT

typedef int coor_t;
typedef coor_t coor_tile_t;
typedef coor_t coor_in_t;
typedef coor_t quat_t;

typedef long long coor_double_t;
typedef coor_double_t scale_t;
typedef coor_double_t coor_world_t;

typedef int trig_t;
typedef int trig_sign_t;

static const coor_double_t COOR_T_MAX = 0x7fffffff;
static const short COOR_SHIFT = 16;
static const short SCALE_SHIFT = 16;

static const coor_double_t COOR_T_MIN = -COOR_T_MAX - 1;
static const coor_t COOR_RES = (1 << COOR_SHIFT);

static const float COOR_SCALE = 1.0f / float(COOR_RES);

static const short QUAT_SHIFT = COOR_SHIFT;
static const short QUAT_HALF_SHIFT = (COOR_SHIFT >> 1);
static const quat_t QUAT_RES = COOR_RES;

static const short COOR_DOUBLE_SHIFT = (COOR_SHIFT << 1);
static const coor_double_t COOR_DOUBLE_RES = ((coor_double_t)(1) << COOR_DOUBLE_SHIFT);

static const scale_t SCALE_RES = (1 << SCALE_SHIFT);


#endif // SE_FIXED_POINT

#endif
