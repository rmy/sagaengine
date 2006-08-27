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


#ifndef IoSchema_hpp
#define IoSchema_hpp

#include "../parse/Parser.hpp"
#include "../encode/io_encode.hpp"
#include "../stream/io_stream.hpp"

namespace se_core {
	/**
	 * Global objects and methods for the core io system.
	 */
	namespace IoSchema {
		extern _SeCoreExport Parser& parser();
		Encoder& encoder();
		extern _SeCoreExport FileManager* fileManager;

		bool _SeCoreExport init();
		void _SeCoreExport cleanup();
		
		/** Force linking of dependencies */
		void _SeCoreExport touch();

		extern _SeCoreExport const char* dataPath;
	}
}


#endif
