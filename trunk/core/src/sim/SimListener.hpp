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


#ifndef SimListener_hpp
#define SimListener_hpp

namespace se_core {
	/**
	 * The Engine will never call higher level system modules.
	 * Instead rendering, input device parsers, etc, hook into
	 * the engine with listeners.
	 *
	 * Classes that want to recieve engine events should subclass
	 * the SimListener class and register themselves as listeners
	 * in the SimSchema.
	 */
	class _SeCoreExport SimListener {
	public:
		virtual ~SimListener() {} // Gets rid of warning

		/**
		 * This callback will be activated at least once
		 * every time the AI has caught up.
		 */
		virtual void renderEvent(long when) = 0;

		/**
		 * This callback will be activated before step() actions are
		 * perform()ed. This may be a good place for reading input
		 * devices and such.
		 */
		virtual void preSimTickEvent(long when) = 0;

		/**
		 * This callback will be activated after step() actions
		 * are done. This is the callback where rendering or
		 * graphics engine state should be performed.
		 */
		virtual void postSimTickEvent(long when) = 0;
	};

}

#endif
