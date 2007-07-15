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


#ifndef Parser_hpp
#define Parser_hpp

#include "../stream/InputStream.hpp"
#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "io_parse.hpp"


namespace se_core {

	class _SeCoreExport ParseManager {
	public:
		ParseManager();
		~ParseManager();
		void add(ParserModule *module);
		void add(ComponentParserModule *module);
		bool parse(InputStream& in);
		ComponentFactory* parseComponent(InputStream& in, int type, int subtype);

	private:
		static const int MAX_ELEMENTS = 48;
		static const int MAX_COMPONENT_ELEMENTS = 48;
		int moduleCount_;
		int componentModuleCount_;
		ParserModule* modules_[ MAX_ELEMENTS ];
		ComponentParserModule* componentModules_[ MAX_COMPONENT_ELEMENTS ];
	};

}

#endif
