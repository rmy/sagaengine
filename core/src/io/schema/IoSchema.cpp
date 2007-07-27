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


#include "IoSchema.hpp"
#include "../parse/all.hpp"
#include "../encode/EncodeManager.hpp"
#include "../stream/FileManager.hpp"
#include "sim/schema/SimSchema.hpp"

namespace se_core {
	namespace IoSchema {
		ParseManager& parser() {
			static ParseManager parser;
			return parser;
		}

		EncodeManager& encoder() {
			static EncodeManager encoder;
			return encoder;
		}

		// Platform dependent layer must override
		// FileManager and register its subclass here
		FileManager* fileManager = 0;

		const char* saveName = 0;
		const char* dataPath = 0;


		bool init() {
			// Create parser manager object
			parser();

			// Create encoder object
			encoder();

			// Create a few file parsers
			static CutsceneParser cpm(IoSchema::parser());
			static DictionaryParser dpm(IoSchema::parser());
			static EmbeddedParser epm(IoSchema::parser());
			static CompositeParser cspm(IoSchema::parser());

			// Return success
			return true;
		}

		void cleanup() {
		}


		void touch() {
			SimSchema::touch();
			parser();
			encoder();
		}
	}
}
