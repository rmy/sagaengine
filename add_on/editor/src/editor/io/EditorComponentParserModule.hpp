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

#include "sim/sim.hpp"
#include "io/parse/ComponentParserModule.hpp"
#include "sim/stat/DictionaryEntry.hpp"

namespace se_editor {
	class _SeEditorExport EditorComponentParserModule : public se_core::ComponentParserModule {
	public:
		EditorComponentParserModule();
		virtual ~EditorComponentParserModule();
		se_core::ComponentFactory* parse(se_core::InputStream& in);

	protected:
		const se_core::DictionaryEntry dict_;
		enum { TYPE = se_core::sct_EDITOR, SUBTYPE = 0 };
	};

	extern _SeEditorExport const EditorComponentParserModule parserEditorComponent;
}

#endif