#include <fbxsdk.h>
#include "DisplayCommon.hpp"
#include "SaveCommon.hpp"
#include <cstring>
#include <cstdio>


#include <fbxfilesdk_nsuse.h>


void SaveHeader(char ch1, char ch2, short ver , FILE* out) {
	fwrite(&ch1, sizeof(ch1), 1, out);
	fwrite(&ch2, sizeof(ch2), 1, out);
	char buffer[3];
	sprintf(buffer, "%02d", ver);
	fwrite(buffer, 2, 1, out);
}


void SaveChar(char v, FILE* out) {
	fwrite(&v, sizeof(v), 1, out);
}


void SaveString(const char* v, FILE* out) {
	fwrite(v, strlen(v) + 1, 1, out);
	const char* buffer = "\0\0\0\0";
	fwrite(buffer, (4 - ((strlen(v) + 1) & 0x3)) & 0x3, 1, out);
}


void SaveShort(short v, FILE* out) {
	fwrite(&v, sizeof(v), 1, out);
}


void SaveFloat(float v, FILE* out) {
	fwrite(&v, sizeof(v), 1, out);
}


void SaveVector(KFbxVector4& v, FILE* out) {
	for(int i = 0; i < 3; ++i) {
		SaveFloat(v[i], out);
	}
}


void SaveVoid(void* v, int size, FILE* out) {
	DisplayInt("        SaveVoid      ", size);
	fwrite(v, size, 1, out);
}

