#ifndef _DISPLAY_COMMON_HPP
#define _DISPLAY_COMMON_HPP

#include <kaydara.h>
#include <fbxfilesdk_def.h>

class FBXFILESDK_NAMESPACE::KFbxVector2;
class FBXFILESDK_NAMESPACE::KFbxVector4;
class FBXFILESDK_NAMESPACE::KFbxColor;

void DisplayString(char* pHeader, const char* pValue  = "", char* pSuffix  = "");
void DisplayBool(char* pHeader, bool pValue, char* pSuffix  = "");
void DisplayInt(char* pHeader, int pValue, char* pSuffix  = "");
void DisplayDouble(char* pHeader, double pValue, char* pSuffix  = "");
void Display2DVector(char* pHeader, FBXFILESDK_NAMESPACE::KFbxVector2 pValue, char* pSuffix  = "");
void Display3DVector(char* pHeader, FBXFILESDK_NAMESPACE::KFbxVector4 pValue, char* pSuffix  = "");
void DisplayColor(char* pHeader, FBXFILESDK_NAMESPACE::KFbxColor pValue, char* pSuffix  = "");


#endif // #ifndef _DISPLAY_COMMON_H

