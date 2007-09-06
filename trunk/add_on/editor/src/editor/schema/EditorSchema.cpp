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


#include "EditorSchema.hpp"
#include "editor/comp/EditorManager.hpp"
#include "sim/schema/SimSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"
#include "comp/Composite.hpp"

using namespace se_core;

namespace se_editor {
	namespace EditorSchema {
		se_core::Composite* lastSpawn = 0;
		CameraPos cameraPos = cam_ABOVE_HIGH;

		class _SeBasicExport AutoInit : public se_core::InitListener {
		public:
			AutoInit() {

				// Register as init event listener
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Editor add-on");
			}

			~AutoInit() {
				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Editor add-on");
			}

			bool initEngineEvent() {
				EditorManager::singleton();
				return true; 
			}
			void cleanupEngineEvent() {}

			bool initGameEvent() {
				lastSpawn = 0;
				return true;
			}
			void cleanupGameEvent() {
			}

			bool initLevelEvent() { return true; }
			void cleanupLevelEvent() {}

		} autoInit;
	}

}
