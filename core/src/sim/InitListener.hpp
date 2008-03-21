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


#ifndef InitListener_hpp
#define InitListener_hpp

namespace se_core {
	/**
	 * The Engine will never call higher level system modules.
	 * Instead rendering, input device parsers, etc, hook into
	 * the engine with listeners.
	 *
	 * Classes that want to recieve engine initialisation events should
	 * subclass the InitListener class and register themselves as listeners
	 * in the InitSchema.
	 */
	class _SeCoreExport InitListener {
	public:
		virtual ~InitListener() {} // Gets rid of warning

		virtual bool priorityInitEngineEvent() { return true; }
		virtual void priorityCleanupEngineEvent() {}
		/**
		 * This event occurs when the core engine has initialised itself.
		 */
		virtual bool initEngineEvent() = 0;

		/**
		 * This event occurs when the core simulation engine is about to
		 * clean up itself.
		 */
		virtual void cleanupEngineEvent() = 0;

		virtual bool initLevelEvent() = 0;
		virtual void cleanupLevelEvent() {}

		/**
		 * This event occurs when the SimEngine has initialised itself
		 * for a new game (SimEngine::initGame() method.)
		 */
		virtual bool initGameEvent() = 0;

		/**
		 * This event occurs when the SimEngine has cleaned up itself
		 * after a game (SimEngine::cleanupGame() method.)
		 */
		virtual void cleanupGameEvent() = 0;

		virtual bool pauseGameEvent() { return true; }
		virtual void unpauseGameEvent() {}

		virtual bool startGameEvent() { return true; }
		virtual void stopGameEvent() {}

	};

}

#endif
