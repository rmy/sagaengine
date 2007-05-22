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


#include "OgreUiPre.hpp"
#include "UiSchema.hpp"
#include "../init/UiInitHandler.hpp"
#include "../material/Sunlight.hpp"
#include "../input/FlyControls.hpp"
#include "util/error/Log.hpp"

using namespace se_ogre;

namespace ui {

	namespace UiSchema {

		// Create a game controller object
		FlyControls flyControls;

		const struct AutoInit {
			AutoInit() {
				static UiInitHandler initHandler;
				static Sunlight sunlight;


				LogDetail("Registered UiSchema game module");
			}

			~AutoInit() {
				LogDetail("Cleaned up UiSchema game module");
			}
		} autoInit;
	}

	/*
	bool UiSchema
	::init() {
		if(!initSeModule_Ogre()) {
			return false;
		}

		// Create a game controller object
		FlyControls simpleControls;

		// Make sure that init calls to se_core::SimEngine is propagated
		static UiInitHandler initHandler;
		static Sunlight sunlight;

		// Success
		return true;


	}


	void UiSchema
	::cleanup() {
		//
		cleanupSeModule_Ogre();
	}
	*/

}
