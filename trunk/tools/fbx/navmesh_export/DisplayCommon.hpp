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


#ifndef _DISPLAY_COMMON_HPP
#define _DISPLAY_COMMON_HPP

#include <kaydara.h>
#include <kfbxplugins/kfbxnamespacedef.h>

class FBXSDK_NAMESPACE::KFbxVector2;
class FBXSDK_NAMESPACE::KFbxVector4;
class FBXSDK_NAMESPACE::KFbxColor;

void DisplayString(char* pHeader, const char* pValue  = "", char* pSuffix  = "");
void DisplayBool(char* pHeader, bool pValue, char* pSuffix  = "");
void DisplayInt(char* pHeader, int pValue, char* pSuffix  = "");
void DisplayDouble(char* pHeader, double pValue, char* pSuffix  = "");
void Display2DVector(char* pHeader, FBXSDK_NAMESPACE::KFbxVector2 pValue, char* pSuffix  = "");
void Display3DVector(char* pHeader, FBXSDK_NAMESPACE::KFbxVector4 pValue, char* pSuffix  = "");
void DisplayColor(char* pHeader, FBXSDK_NAMESPACE::KFbxColor pValue, char* pSuffix  = "");


#endif // #ifndef _DISPLAY_COMMON_H

