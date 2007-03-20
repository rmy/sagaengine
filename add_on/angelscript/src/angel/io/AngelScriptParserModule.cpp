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


#include "AngelScriptParserModule.hpp"
#include "../schema/AngelSchema.hpp"
#include "../script/AngelScript.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"
#include "io/stream/InputStream.hpp"
#include "io/schema/IoSchema.hpp"
#include <angelscript.h>
#include <cstring>


namespace se_core {

	/**
	 * Always start angel script files with "//01 name"
	 */
	AngelScriptParserModule
	::AngelScriptParserModule(Parser& parser)
			: ParserModule(parser, '/', '*', 1), destroyCount_(0)  {
	}

	AngelScriptParserModule
	::~AngelScriptParserModule() {
		for(int i = 0; i < destroyCount_; ++i)
			delete destroyList_[ i ];
		destroyCount_ = 0;
	}


	void AngelScriptParserModule
	::parse(InputStream& in) {
		String* name = new String();
		name->copy( in.basename() );
		LogWarning(name);

		// Read end of comment ( star + slash )
		String tmp;
		do {
			in.readString(tmp);
		} while(!tmp.equals("*/"));


		const int BUFFER_SIZE = 2048;
		char script[ BUFFER_SIZE + 1 ];
		int len = 0;

		while(!in.eof() && len + 1 < BUFFER_SIZE) {
			in.readLine(&script[ len ], BUFFER_SIZE - len);
			len += strlen(&script[ len ]);
		}

		int r;
		r = AngelSchema::scriptEngine->AddScriptSection(name->get(), "script", script, strlen(script), 0, false);
		if( r < 0 ) {
			LogFatal("AddScriptSection() failed: " << name->get());
		}

		r = AngelSchema::scriptEngine->Build(name->get());
		if( r < 0 ) {
			LogFatal("Build() failed: " << name->get());
		}

		// Create and register script
		AngelScript* s = new AngelScript(name);

		// Add script to destruction list
		destroyList_[ destroyCount_++ ] = s;
	}

}
