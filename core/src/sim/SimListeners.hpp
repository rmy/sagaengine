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


#ifndef SimListeners_hpp
#define SimListeners_hpp

#include "SimListener.hpp"
#include "sim.hpp"
#include "../util/error/Log.hpp"

namespace se_core {

	class _SeCoreExport SimListeners {
	public:
		/**
		 * Add an engine listener.
		 */
		void addListener(SimListener &l) {
			Assert(listenerCount < MAX_LISTENERS && "Too many listeners. Ajust MAX_LISTENERS.");
			listeners[ listenerCount++ ] = &l;
		}


		/**
		 * Remove an engine listener.
		 */
		void removeListener(SimListener &l) {
			for(int i = 0; i < listenerCount; ++i) {
				if(listeners[ i ] == &l) {
					listeners[ i ] = listeners[ --listenerCount ];
				}
			}
		}

		/**
		 * Cast a pre step event. Done by the Engine class from inside
		 * the step() method.
		 */
		void castPreSimTickEvent(long when) {
			for(int i = 0; i < listenerCount; ++i) {
				listeners[ i ]->preSimTickEvent(when);
			}
		}

		/**
		 * Cast a pre step event. Done by the Engine class from inside
		 * the step() method.
		 */
		void castRenderEvent(long when) {
			for(int i = 0; i < listenerCount; ++i) {
				listeners[ i ]->renderEvent(when);
			}
		}


		/**
		 * Cast a post step event. Done by the Engine class from inside
		 * the step() method.
		 */
		void castPostSimTickEvent(long when) {
			for(int i = 0; i < listenerCount; ++i) {
				listeners[ i ]->postSimTickEvent(when);
			}
		}

		/**
		 * Remove all listeners.
		 */
		void clear() {
			listenerCount = 0;
		}


	private:
		/** Number of listeners the this container is dimensioned for */
		static const int MAX_LISTENERS = 10;
		/** An array containg all the engine listeners */
		SimListener* listeners[ MAX_LISTENERS ];
		/** The number of listeners presently in the container */
		short listenerCount;
	};

}

#endif
