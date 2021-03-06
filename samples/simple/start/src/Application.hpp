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


#ifndef Application_hpp
#define Application_hpp


namespace logic {
	class Application {
	public:
		Application(const char* appName);
		~Application();

		/**
		 * Initialise things that need to be initialised only once
		 * during the lifetime of the application.
		 */
		bool initEngine(const char* appName);

		/**
		 * Initialise things that need to be reinitialised every
		 * time a new game is started.
		 */
		bool initGame();

		/**
		 * Execute the game.
		 */
		void go();

		/**
		 * Cleanup the after game.
		 */
		void cleanupGame();

		/**
		 * Cleanup before shutting down the application.
		 */
		void cleanupEngine();

	private: // Helper methods
	};
}

#endif
