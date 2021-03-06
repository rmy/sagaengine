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


#include "ParseManager.hpp"
#include "Parser.hpp"
#include "component/ComponentParser.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_core {

	ParseManager
	::ParseManager()
		: moduleCount_(0), componentModuleCount_(0) {
	}


	ParseManager
	::~ParseManager() {
	}


	void ParseManager
	::add(Parser *module) {
		LogDetail("Added file parser for header code: " << module->headerCodeString());
		for(int i = 0; i < moduleCount_; ++i) {
			if(module->headerCode() == modules_[ i ]->headerCode()) {
				LogFatal("Module with header code " << module->headerCodeString() << " already exists");
				return;
			}
		}
		modules_[ moduleCount_++ ] = module;
	}

	void ParseManager
	::add(ComponentParser *module) {
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

	bool ParseManager
	::parse(InputStream& in) {
		LogWarning("Loading file: " << in.name());

		int headerCode = in.readHeaderCode();

		for(int i = 0; i < moduleCount_; ++i) {
			if(modules_[i]->headerCode() == headerCode) {
				modules_[i]->parse(in);
				return true;
			}
		}
		LogWarning("The file '" << in.name() << "' had unsupported header name: " << headerCode);
		return false;
	}

	ComponentFactory* ParseManager
	::parseComponent(InputStream& in, int type, int subtype) {
		//LogDetail("Loading component type " << type << ", " << subtype << " for " << in.name());
		LogWarning("Loading component type " << type << ", " << subtype << " for " << in.name());

		for(int i = 0; i < componentModuleCount_; ++i) {
			if(type == componentModules_[ i ]->type()
					&& subtype == componentModules_[ i ]->subtype()) {
				ComponentFactory* f = componentModules_[i]->parse(in);
				if(f) f->setDoDestroy(true);
				return f;
			}
		}
		LogWarning("The file '" << in.name() << "' had unsupported component type: " << type << ", " << subtype << ".");
		return 0;
	}
};
