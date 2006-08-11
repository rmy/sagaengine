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


#include "Parser.hpp"
#include "ParserModule.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	Parser
	::Parser() : moduleCount_(0) {
	}


	Parser
	::~Parser() {
	}


	void Parser
	::add(ParserModule *module) {
		LogMsg("Added header code: " << module->headerCodeString());
		for(int i = 0; i < moduleCount_; ++i) {
			if(module->headerCode() == modules_[ i ]->headerCode()) {
				LogFatal("Adding module with existing header code.");
				return;
			}
		}
		modules_[ moduleCount_++ ] = module;
	}


	void Parser
	::parse(InputStream& in, const char* msgOnError) {
		LogMsg(in.name());

		int headerCode = in.readHeaderCode();

		//printf("Header code: %d %x\n", headerCode, headerCode);
		LogMsg(msgOnError);

		for(int i = 0; i < moduleCount_; ++i) {
			if(modules_[i]->headerCode() == headerCode) {
				modules_[i]->parse(in);
				return;
			}
		}
		LogFatal(msgOnError);
		//LogFatal("Tried to parse file with unsupported header code");
	}

};