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


#include "PluginManager.hpp"
#include "util/error/Log.hpp"
#include <cstring>

namespace se_plugin {

	PluginManager
	::PluginManager() 
		: plugins_(new Plugin*[MAX_PLUGINS]), pluginCount_(0) {
	}


	PluginManager
	::~PluginManager() {
		unloadAll();
	}


	Plugin* PluginManager
	::load(const char* name) {
		for(int i = 0; i < pluginCount_; ++i) {
			if(strcmp(plugins_[i]->name(), name) == 0) {
				LogFatal("Plugin with this name already loaded: " << name);
				return plugins_[i];
			}
		}

		plugins_[ pluginCount_ ] = new Plugin(name);
		plugins_[ pluginCount_ ]->load();

		return plugins_[ pluginCount_++ ];
	}


	void PluginManager
	::unload(Plugin* plugin) {
		// Find and unload plugin
		short foundAt = pluginCount_;
		for(int i = 0; i < pluginCount_; ++i) {
			if(plugins_[i]  == plugin) {
				foundAt = i;
				plugin->unload();
				delete plugin;
				break;
			}
		}

		Assert(foundAt < pluginCount_ && "Plugin not found for unloading");

		// Remove from list - maintain order
		for(int i = foundAt; i < pluginCount_; ++i) {
			plugins_[ i ] = plugins_[ i + 1 ];
		}

		// One less
		--pluginCount_;
	}

	void PluginManager
	::unloadAll() {
		// Unload in reverse loading order
		while(pluginCount_ > 0) {
			plugins_[--pluginCount_]->unload();
		}
	}

}
