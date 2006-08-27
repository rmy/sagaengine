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
	public:
		InitListeners() 
			: listenerCount_(0), nextEngineInit_(0), didAlreadyInitEngine_(false) {
		}


		/**
		 * Add an engine listener.
		 */
		void addListener(InitListener &l) {
			Assert(listenerCount_ < MAX_LISTENERS && "Too many listeners. Ajust MAX_LISTENERS.");
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

		void castInitEngineEvent() {
			int i = nextEngineInit_;
			while(i < listenerCount_) {
				listeners_[ i ]->priorityInitEngineEvent();
				++i;
			}
			while(nextEngineInit_ < listenerCount_) {
				listeners_[ nextEngineInit_ ]->initEngineEvent();
				++nextEngineInit_;
			}
			didAlreadyInitEngine_ = true;
		}

		void castCleanupEngineEvent() {
			for(int i = 0; i < listenerCount_; ++i) {
				listeners_[ i ]->cleanupEngineEvent();
			}
		}

		void castInitGameEvent() {
			for(int i = 0; i < listenerCount_; ++i) {
				listeners_[ i ]->initGameEvent();
			}
		}

		void castCleanupGameEvent() {
			for(int i = 0; i < listenerCount_; ++i) {
				listeners_[ i ]->cleanupGameEvent();
			}
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

		short nextEngineInit_;
		/** Did already init engine. */
		bool didAlreadyInitEngine_;
	};

}

#endif
