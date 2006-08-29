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

using namespace se_core;
using namespace se_ogre;

namespace ui {

	namespace UiSchema {
		// Create a game controller object
		GameControls gameControls;

		const struct AutoInit : public se_core::InitListener {
			AutoInit() {
				static Sunlight sunlight;

				SimSchema::initListeners().addListener(*this);
				LogMsg("Registered UiSchema game module");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogMsg("Cleaned up UiSchema game module");
			}


			void initEngineEvent() {
			}


			void cleanupEngineEvent() {
			}


			void initGameEvent() {
				UiSchema::gameControls.grabFocus();
			}


			void cleanupGameEvent() {
			}


		} autoInit;

	}

}
