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


#ifndef ComponentParser_hpp
#define ComponentParser_hpp

#include "io/parse/ComponentParserModule.hpp"
#include "sim/stat/DictionaryEntry.hpp"

namespace %n {
	class _%NExport %sComponentParserModule : public se_core::ComponentParserModule {
	public:
		%sComponentParserModule();
		virtual ~%sComponentParserModule();
		se_core::SimComponentFactory* parse(se_core::InputStream& in);

	protected:
		const se_core::DictionaryEntry dict_;
		enum { TYPE = se_core::sct_%S, SUBTYPE = 0 };
	};

	extern _%NExport const %sComponentParserModule parser%sComponent;
}

#endif
