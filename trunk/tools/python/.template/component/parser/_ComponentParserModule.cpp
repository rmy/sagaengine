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


#include "%sComponentParserModule.hpp"
#include "sim/schema/SimSchema.hpp"
#include "io/parse/Parser.hpp"
#include "io/schema/IoSchema.hpp"
#include "util/error/Log.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "../%_/%sComponentFactory.hpp"

using namespace se_core;

namespace %n {
	const %sComponentParserModule parser%sComponent;

	%sComponentParserModule
	::%sComponentParserModule()
			: ComponentParserModule(IoSchema::parser(), TYPE, SUBTYPE)
			, dict_(se_core::DE_COMPONENT_TYPE, TYPE, "%S") {
	}

	
	%sComponentParserModule
	::~%sComponentParserModule() {
	}


	ComponentFactory* %sComponentParserModule
	::parse(InputStream& in) {
		%sComponentFactory* factory = new %sComponentFactory();

		int code = in.readInfoCode();
		Assert(code == '{');

		while((code = in.readInfoCode()) != '}') {
			switch(code) {
			default:
				LogFatal("Unsupported code '" << (char)(code) << "' in file " << in.name());
			}
		}

		return factory;
	}

}
