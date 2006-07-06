#ifndef _SAVE_COMMON_HPP
#define _SAVE_COMMON_HPP

#include <kaydara.h>
#include <fbxfilesdk_def.h>

class FBXFILESDK_NAMESPACE::KFbxVector2;
class FBXFILESDK_NAMESPACE::KFbxVector4;
class FBXFILESDK_NAMESPACE::KFbxColor;

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
void SaveVector(FBXFILESDK_NAMESPACE::KFbxVector4& v, FILE* out);

#endif
