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


#ifndef game_app_Application_hpp
#define game_app_Application_hpp


namespace ui { class Ui; }

namespace game {
	class Application {
	public:
		Application();
		~Application();

		bool init();
		void go();
		void cleanup();

	private: // Helper methods

		/**
		 * Basic initialisation of the SagaEngine modules
		 * that we want to use.
		 */
		bool initModules();
		void cleanupModules();

		/**
		 * Initialisation of SagaEngine core
		 * which is done only once after the application
		 * is started up.
		 */
		bool initSECore();


		/**
		 * Cleanup of SagaEngine core that is done when appliaction
		 * exits.
		 */
		void cleanupSECore();


		class ui::Ui* ui_;
	};
}

#endif
