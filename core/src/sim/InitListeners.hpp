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


#ifndef InitListeners_hpp
#define InitListeners_hpp

#include "InitListener.hpp"
#include "sim.hpp"
#include "../util/error/Log.hpp"

namespace se_core {

	class _SeCoreExport InitListeners {
		enum { init_PRIORITY_ENGINE, init_ENGINE, init_LEVEL, init_GAME, init_START, init_PAUSE, init_COUNT };
	public:

		InitListeners()
				: listenerCount_(0), didAlreadyInitEngine_(false) {
			for(int i = 0; i < init_COUNT; ++i) {
				nextEngineInit_[i] = 0;
			}
		}


		/**
		 * Add an engine listener.
		 */
		void addListener(InitListener &l) {
			AssertFatal(listenerCount_ < MAX_LISTENERS, "Too many listeners. Ajust MAX_LISTENERS.");
			listeners_[ listenerCount_++ ] = &l;
		}


		/**
		 * Remove an engine listener.
		 */
		void removeListener(InitListener &l) {
			for(int i = 0; i < listenerCount_; ++i) {
				if(listeners_[ i ] == &l) {
					listeners_[ i ] = listeners_[ --listenerCount_ ];
				}
			}
		}

		bool castPriorityInitEngineEvent() {
			int &i = nextEngineInit_[ init_PRIORITY_ENGINE ];
			while(i < listenerCount_) {
				if(!listeners_[ i ]->priorityInitEngineEvent()) {
					castPriorityCleanupEngineEvent();
					return false;
				}
				++i;
			}
			return true;
		}

		void castPriorityCleanupEngineEvent() {
			int &i = nextEngineInit_[ init_PRIORITY_ENGINE ];
			while(i > 0) {
				--i;
				listeners_[ i ]->priorityCleanupEngineEvent();
			}
		}

		bool castInitEngineEvent() {
			int &i = nextEngineInit_[ init_ENGINE ];
			while(i < listenerCount_) {
				if(!listeners_[ i ]->initEngineEvent()) {
					castCleanupEngineEvent();
					return false;
				}
				++i;
			}
			didAlreadyInitEngine_ = true;
			return true;
		}

		void castCleanupEngineEvent() {
			int &i = nextEngineInit_[ init_ENGINE ];
			while(i > 0) {
				--i;
				listeners_[ i ]->cleanupEngineEvent();
			}
		}


		bool castInitLevelEvent() {
			int &i = nextEngineInit_[ init_LEVEL ];
			while(i < listenerCount_) {
				if(!listeners_[ i ]->initLevelEvent()) {
					castCleanupLevelEvent();
					return false;
				}
				++i;
			}
			return true;
		}

		void castCleanupLevelEvent() {
			int &i = nextEngineInit_[ init_LEVEL ];
			while(i > 0) {
				--i;
				listeners_[ i ]->cleanupLevelEvent();
			}
		}


		bool castInitGameEvent() {
			int &i = nextEngineInit_[ init_GAME ];
			while(i < listenerCount_) {
				if(!listeners_[ i ]->initGameEvent()) {
					castCleanupGameEvent();
					return false;
				}
				++i;
			}
			return true;
		}

		void castCleanupGameEvent() {
			int &i = nextEngineInit_[ init_GAME ];
			while(i > 0) {
				--i;
				listeners_[ i ]->cleanupGameEvent();
			}
		}


		bool castStartGameEvent() {
			int &i = nextEngineInit_[ init_START ];
			while(i > 0) {
				--i;
				if(!listeners_[ i ]->startGameEvent()) {
					castStopGameEvent();
					return false;
				}
			}
			return true;
		}

		void castStopGameEvent() {
			int &i = nextEngineInit_[ init_START ];
			while(i > 0) {
				--i;
				listeners_[ i ]->stopGameEvent();
			}
		}


		bool castPauseGameEvent() {
			int &i = nextEngineInit_[ init_PAUSE ];
			LogWarning("Pause count: " << i);
			while(i < listenerCount_) {
				if(!listeners_[ i ]->pauseGameEvent()) {
					LogFatal("What happened?");
					castUnpauseGameEvent();
					return false;
				}
				++i;
			}
			LogWarning("Pause count: " << i);
			return true;
		}

		void castUnpauseGameEvent() {
			int &i = nextEngineInit_[ init_PAUSE ];
			LogWarning("Pause count: " << i);
			while(i > 0) {
				--i;
				listeners_[ i ]->unpauseGameEvent();
			}
			LogWarning("Pause count: " << i);
		}


		/**
		 * Remove all listeners.
		 */
		void clear() {
			listenerCount_ = 0;
			didAlreadyInitEngine_ = false;
		}


	private:
		/** Number of listeners the this container is dimensioned for */
		static const int MAX_LISTENERS = 10;
		/** An array containg all the engine listeners */
		InitListener* listeners_[ MAX_LISTENERS ];
		/** The number of listeners presently in the container */
		short listenerCount_;

		int nextEngineInit_[init_COUNT];
		/** Did already init engine. */
		bool didAlreadyInitEngine_;
	};

}

#endif
