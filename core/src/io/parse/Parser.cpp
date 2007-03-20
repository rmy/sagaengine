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
#include "ComponentParserModule.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	Parser
	::Parser()
		: moduleCount_(0), componentModuleCount_(0) {
	}


	Parser
	::~Parser() {
	}


	void Parser
	::add(ParserModule *module) {
		LogDetail("Added file parser for header code: " << module->headerCodeString());
		for(int i = 0; i < moduleCount_; ++i) {
			if(module->headerCode() == modules_[ i ]->headerCode()) {
				LogFatal("Module with header code " << module->headerCodeString() << " already exists");
				return;
			}
		}
		modules_[ moduleCount_++ ] = module;
	}

	void Parser
	::add(ComponentParserModule *module) {
		LogDetail("Added file component parser for type code: " << module->type() << ", " << module->subtype());
		for(int i = 0; i < componentModuleCount_; ++i) {
			if(module->type() == componentModules_[ i ]->type()
					&& module->subtype() == componentModules_[ i ]->subtype()) {
				LogFatal("Component module with header code " << module->type()  << ", " << module->subtype() << " already exists");
				return;
			}
		}
		componentModules_[ componentModuleCount_++ ] = module;
	}

	bool Parser
	::parse(InputStream& in) {
		LogDetail("Loading file: " << in.name());

		int headerCode = in.readHeaderCode();

		for(int i = 0; i < moduleCount_; ++i) {
			if(modules_[i]->headerCode() == headerCode) {
				modules_[i]->parse(in);
				return true;
			}
		}
		LogWarning("The file '" << in.name() << "' had unsupported header name.");
		return false;
	}

	SimComponentFactory* Parser
	::parseComponent(InputStream& in, int type, int subtype) {
		LogDetail("Loading component type " << type << ", " << subtype << " for " << in.name());

		for(int i = 0; i < componentModuleCount_; ++i) {
			if(type == componentModules_[ i ]->type()
					&& subtype == componentModules_[ i ]->subtype()) {
				return componentModules_[i]->parse(in);
			}
		}
		LogWarning("The file '" << in.name() << "' had unsupported component type: " << type << ", " << subtype << ".");
		return 0;
	}
};
