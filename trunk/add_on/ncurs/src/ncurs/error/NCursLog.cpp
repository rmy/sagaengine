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


#include "NCursLog.hpp"
#include "util/type/TmpString.hpp"
#include "../gfx/canvas/Canvas.hpp"
#include "../schema/NCursSchema.hpp"
#include <cstdio>
#include <csignal>


using namespace se_core;
using namespace se_ncurs;

namespace se_err {

	void scream3(const char* file, int line, const char* msg) {
		char* buffer = new char[512];
		sprintf(buffer, "Fat: %s-%d: \"%s\"", file, line, msg);
		NCursSchema::canvas().say(buffer);
		fputs(buffer, stderr);
		fputs("\n", stderr);
		//puts(buffer);
		raise(11);
		throw buffer;
	}


	void say3(const char* file, int line, const char* msg) {
		// Msg
		char buffer[512];
		sprintf(buffer, "Msg: %s-%d: \"%s\"", file, line, msg);
		NCursSchema::canvas().say(buffer);
		fputs(buffer, stderr);
		fputs("\n", stderr);
		//puts(buffer);
	}


	void dump(const char *msg) {
		// Msg, not debug
		NCursSchema::canvas().say(msg);
		fputs(msg, stderr);
		fputs("\n", stderr);
		//puts(msg);
	}
}
