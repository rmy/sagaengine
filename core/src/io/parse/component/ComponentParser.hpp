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

#include "../io_parse.hpp"
#include "../../stream/io_stream.hpp"
#include "comp/ComponentFactory.hpp"
#include "sim/stat/DictionaryEntry.hpp"

namespace se_core {
	class _SeCoreExport ComponentParser {
	public:
		ComponentParser(ParseManager &parser, int type, int subtype);
		virtual ~ComponentParser() {}
		int type() { return type_; }
		int subtype() { return subType_; }
		virtual ComponentFactory* parse(InputStream& in) = 0;

	private:
		int type_, subType_;
	};

}

#endif
