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


#include "StopWatch.hpp"
#include <cstdio>
#include <cstring>

namespace se_core {

	StopWatch
	::StopWatch() :
		n(0) {
		for(int i = 0; i < 10; ++i) {
			maxUsed[i] = 0;
		}
	}

	char* StopWatch
	::millisUsedAsString() {
		static char report[80];
		strcpy(report, "Millis:");
		for(int i = 0; i < n; ++i) {
			//sprintf(&report[strlen(report)], " %6.3f", maxMillisUsed(i));
			sprintf(&report[strlen(report)], " %6.3f", millisUsed(i));
		}
		n = 0;
		return report;
	}

	/*
	long StopWatch
	::currentSystemTime() {
		unsigned short* REG_TM2D = (unsigned short*)0x04000108;
		unsigned short* REG_TM3D = (unsigned short*)0x0400010c;
		return (((*REG_TM3D) << 16) + (*REG_TM2D));
	}
	*/

	long StopWatch
	::currentSystemTime() {
		static int c = 1;
		return (c++ % 1000);
	}

}
