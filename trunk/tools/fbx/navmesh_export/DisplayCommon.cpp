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


#include <fbxsdk.h>

#include <stdio.h>
#include <math.h>

using namespace FBXSDK_NAMESPACE;

void DisplayString(char* pHeader, const char* pValue /* = "" */, char* pSuffix /* = "" */) {
	KString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void DisplayBool(char* pHeader, bool pValue, char* pSuffix /* = "" */) {
	KString lString;

	lString = pHeader;
	lString += pValue ? "true" : "false";
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void DisplayInt(char* pHeader, int pValue, char* pSuffix /* = "" */) {
	KString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void DisplayDouble(char* pHeader, double pValue, char* pSuffix /* = "" */) {
	KString lString;
	KString lFloatValue = (float) pValue;

	lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
	lFloatValue = pValue >=  HUGE_VAL ?  "INFINITY" : lFloatValue.Buffer();

	lString = pHeader;
	lString += lFloatValue;
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void Display2DVector(char* pHeader, KFbxVector2 pValue, char* pSuffix  /* = "" */) {
	KString lString;
	KString lFloatValue1 = (float)pValue[0];
	KString lFloatValue2 = (float)pValue[1];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : lFloatValue2.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void Display3DVector(char* pHeader, KFbxVector4 pValue, char* pSuffix /* = "" */) {
	KString lString;
	KString lFloatValue1 = (float)pValue[0];
	KString lFloatValue2 = (float)pValue[1];
	KString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >=  HUGE_VAL ?  "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >=  HUGE_VAL ?  "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >=  HUGE_VAL ?  "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}


void DisplayColor(char* pHeader, KFbxColor pValue, char* pSuffix /* = "" */) {
	KString lString;

	lString = pHeader;
	lString += (float) pValue.mRed;
	lString += " (red), ";
	lString += (float) pValue.mGreen;
	lString += " (green), ";
	lString += (float) pValue.mBlue;
	lString += " (blue)";
	lString += pSuffix;
	lString += "\n";
	printf(lString);
}
