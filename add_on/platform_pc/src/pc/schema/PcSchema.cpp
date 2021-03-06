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


#include "PcSchema.hpp"
#include "../io/PluginParser.hpp"
#include "../io/PcFileManager.hpp"
#include "sim/InitListener.hpp"
#include "sim/InitListeners.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/schema/IoSchema.hpp"


using namespace se_core;

namespace se_pc {
	namespace PcSchema {
		PluginManager& pluginManager() {
			static PluginManager pluginManager;
			return pluginManager;
		}


		const struct _SePcExport AutoInit : public se_core::InitListener {
			AutoInit() {
				SimSchema::initListeners().addListener(*this);
				LogDetail("Registered Plugin add-on");
			}


			~AutoInit() {
				if(IoSchema::fileManager)
					IoSchema::fileManager->cleanup();
				delete IoSchema::fileManager;
				IoSchema::fileManager = 0;

				SimSchema::initListeners().removeListener(*this);
				LogDetail("Cleaned up Plugin add-on");
			}

			bool priorityInitEngineEvent() {
				// Register a file manager
				// (Could have been a network loader, or anything else.)
				se_core::String dataPath(SimSchema::appName);
				dataPath.append("_datapath.txt");
				IoSchema::fileManager = new PcFileManager(dataPath.get());
				return true;
			}


			bool initEngineEvent() {
				// Register some file loaders
				static PluginParser pluginParser(se_core::IoSchema::parser());
				return true;
			}

			void cleanupEngineEvent() {
			}

			bool initGameEvent() {
				return true;
			}
			void cleanupGameEvent() {}
			bool initLevelEvent() {
				return true;
			}
			void cleanupLevelEvent() {}
		} autoInit;


		void touch() {
			se_core::IoSchema::touch();
			pluginManager();
			autoInit;
		}
	}
}
