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
#include "UiInitHandler.hpp"
#include "../schema/UiSchema.hpp"
#include "../input/FlyControls.hpp"


using namespace se_core;

namespace ui {

	UiInitHandler
	::UiInitHandler() {
		SimSchema::initListeners().addListener(*this);
	}


	UiInitHandler
	::~UiInitHandler() {
		SimSchema::initListeners().removeListener(*this);
	}


	bool UiInitHandler
	::initEngineEvent() {
		return true;
	}


	void UiInitHandler
	::cleanupEngineEvent() {
	}


	bool UiInitHandler
	::initGameEvent() {
		// Create a game controller object
		//gameControls_ = new BlobberControls();

		// Give focus to the game controller
		UiSchema::flyControls.grabFocus();
		return true;
	}


	void UiInitHandler
	::cleanupGameEvent() {
	}

	bool UiInitHandler
	::initLevelEvent() {
		return true;
	}


	void UiInitHandler
	::cleanupLevelEvent() {
	}

}
