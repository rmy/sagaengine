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
#ifdef _WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

using namespace se_core;

namespace se_pc {

	Plugin
	::Plugin(const char* name) : isLoaded_(false) {
#		ifdef _WINDOWS
		name_.copy(name);
#		else
		name_.set("lib");
		name_.append(name);
		name_.append(".so");
#endif
	}


	Plugin
	::~Plugin() {
		if(isLoaded_)
			unload();
	}


	void Plugin
	::load() {
		LogMsg("Loading plugin " << name());
#		ifdef _WINDOWS
		plugin_ = LoadLibrary(name_.get());
#		else
		plugin_ = dlopen(name_.get() , RTLD_LAZY | RTLD_GLOBAL);
#		endif
		if(!plugin_) {
#			ifdef _WINDOWS
			LogFatal("Couldn't load " << name());
#			else
			LogFatal("Couldn't load " << name() << ": " << dlerror());
#endif
		}

		isLoaded_ = (plugin_ != 0);
	}


	void Plugin
	::unload() {
#		ifdef _WINDOWS
		FreeLibrary((HMODULE)plugin_);
#		else
		dlclose( plugin_ );
#		endif
		isLoaded_ = false;		
	}

}
