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


#ifndef PulseaudioManager_hpp
#define PulseaudioManager_hpp

#include "comp/node/RootComponent.hpp"


namespace se_pulseaudio {

	/**
	 * Base class for functionality component managers.
	 */
	class _SePulseaudioExport PulseaudioManager : public se_core::RootComponent {
	public:
		/**
		 * Construct PulseaudioManager.
		 */
		PulseaudioManager();


		/**
		 * Destructor.
		 */
		virtual ~PulseaudioManager();

		const char* name() { return "PulseaudioManager"; }
		

		void step(long when);

		void initGame();
		void cleanupGame();

		static PulseaudioManager& singleton();
	};

}

#endif
