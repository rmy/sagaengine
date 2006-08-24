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


#include "Plugin.hpp"
#include "util/error/Log.hpp"
#include <dlfcn.h>

using namespace se_core;

namespace se_plugin {

	Plugin
	::Plugin(const char* name) : isLoaded_(false) {
		name_.set(name);
		name_.append(".so");
	}


	Plugin
	::~Plugin() {
		if(isLoaded_)
			unload();
	}


	void Plugin
	::load() {
		LogMsg("Loading plugin " << name());
		plugin_ = dlopen(name_.get() , RTLD_LAZY | RTLD_GLOBAL);
		if(!plugin_) {
			LogFatal("Couldn't load " << name() << ": " << dlerror());
		}

		isLoaded_ = (plugin_ != 0);
	}


	void Plugin
	::unload() {
		dlclose( plugin_ );
		isLoaded_ = false;		
	}

}
