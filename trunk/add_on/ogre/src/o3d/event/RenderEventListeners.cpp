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


#include "RenderEventListeners.hpp"
#include "RenderEventListener.hpp"
#include "util/error/Log.hpp"

namespace se_ogre {
	void RenderEventListeners
	::castInitEngine() {
		for(int i = 0; i < listenerCount_; ++i) {
			bool ret = listeners_[i]->initEngineEvent();
			Assert(ret);
		}
	}


	void RenderEventListeners
	::castInitGame() {
		for(int i = 0; i < listenerCount_; ++i) {
			bool ret = listeners_[i]->initGameEvent();
			Assert(ret);
		}
	}


	void RenderEventListeners
	::castInitLevel() {
		for(int i = 0; i < listenerCount_; ++i) {
			bool ret = listeners_[i]->initLevelEvent();
			Assert(ret);
		}
	}


	void RenderEventListeners
	::castCleanupLevel() {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->cleanupLevelEvent();
		}
	}


	void RenderEventListeners
	::castCleanupGame() {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->cleanupGameEvent();
		}
	}


	void RenderEventListeners
	::castCleanupEngine() {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->cleanupEngineEvent();
		}
	}


	void RenderEventListeners
	::castUpdateLights(float gameClock) {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->updateLights(gameClock);
		}
	}


	void RenderEventListeners
	::castUpdateMaterials(float gameClock) {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->updateMaterials(gameClock);
		}
	}


	void RenderEventListeners
	::castUpdateImpostors(float gameClock) {
		for(int i = 0; i < listenerCount_; ++i) {
			listeners_[i]->updateImpostors(gameClock);
		}
	}


	void RenderEventListeners
	::addListener(RenderEventListener* listener) {
		Assert(listenerCount_ < MAX_LISTENERS - 1);
		listeners_[ listenerCount_++ ] = listener;
	}


	void RenderEventListeners
	::removeListener(RenderEventListener* listener) {
		for(int i = 0; i < listenerCount_; ++i) {
			if(listeners_[i] == listener) {
				listeners_[i] = listeners_[ --listenerCount_ ];
				return;
			}
		}
	}

}
