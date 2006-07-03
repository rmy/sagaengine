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


#ifndef _SAVE_COMMON_HPP
#define _SAVE_COMMON_HPP

#include <kaydara.h>
#include <kfbxplugins/kfbxnamespacedef.h>

class FBXSDK_NAMESPACE::KFbxVector2;
class FBXSDK_NAMESPACE::KFbxVector4;
class FBXSDK_NAMESPACE::KFbxColor;

#ifdef SE_FIXED_POINT
// 16.16 fixed point coordinates
typedef int coor_t;
struct CoorT { static coor_t fromFloat(float v) { return (int)(v * 65536); } };
#else
// Floating point coordinates
typedef float coor_t;
struct CoorT { static coor_t fromFloat(float v) { return v; } };
#endif



void SaveHeader(char ch1, char ch2, short ver , FILE* out);
void SaveChar(const char v, FILE* out);
void SaveString(const char* v, FILE* out);
void SaveShort(short v, FILE* out);
void SaveFloat(float v, FILE* out);
void SaveVoid(void* v, int size, FILE* out);
void SaveVector(FBXSDK_NAMESPACE::KFbxVector4& v, FILE* out);

#endif
