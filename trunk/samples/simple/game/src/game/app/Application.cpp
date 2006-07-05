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


#include "GamePre.H"
#include "Application.hpp"
#include "Scripting.hpp"
#include "../schema/GameSchema.hpp"

using namespace se_core;
using namespace ui;

namespace game {
	Application
	::Application() {
	}


	Application
	::~Application() {
	}


	bool Application
	::init() {
		WasHere();
		if(!initModules())
			return false;
		WasHere();
		if(!initSECore())
			return false;

		WasHere();
		ui_ = new Ui();
		WasHere();
		if(!ui_->init())
			return false;

		WasHere();
		return true;
	}


	void Application
	::go() {
		WasHere();
		ui_->go();
		WasHere();
	}


	void Application
	::cleanup() {
		ui_->cleanup();
		delete ui_;
		cleanupSECore();
		cleanupModules();
	}


	bool Application
	::initModules() {
		if(!initSeModule_Core()) {
			// Failure
			return false;
		}

		if(!initSeModule_Client()) {
			return false;
		}

		//if(!initSeModule_Angelscript()) {
		//	return false;
		//}

		if(!GameSchema::init()) {
			WasHere();
			return false;
		}

		if(!initGameModule_PlatformUI()) {
			return false;
		}

		return true;
	}


	void Application
	::cleanupModules() {
		cleanupGameModule_PlatformUI();
		GameSchema::cleanup();
		//cleanupSeModule_Angelscript();
		cleanupSeModule_Client();
		cleanupSeModule_Core();
	}


	bool Application
	::initSECore() {
		if(!SimSchema::init()) {
			return false;
		}

		if(!Scripting::init()) {
			WasHere();
			return false;
		}

		if(!SimEngine::init()) {
			WasHere();
			return false;
		}
		Assert(IoSchema::fileManager);

		WasHere();
		// Load language files
		IoSchema::fileManager->loadDirectory("game/lang");

		// Load scripts
		IoSchema::fileManager->loadDirectory("game/script");

		// Load thing definitions
		IoSchema::fileManager->loadDirectory("game/thing");

		// Load movement data
		IoSchema::fileManager->loadDirectory("game/area/movement/");

		// Load cutscenes
		IoSchema::fileManager->loadDirectory("game/cutscene/");

		return true;
	}


	void Application
	::cleanupSECore() {
		Scripting::cleanup();
		SimEngine::singleton()->cleanup();
	}

}
