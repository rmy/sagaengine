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


#ifndef SpawnComponentParser_hpp
#define SpawnComponentParser_hpp

#include "io/parse/component/ComponentParser.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/sim.hpp"
#include "util/vecmath/util_vecmath.hpp"

namespace se_core {
	class _SeCoreExport SpawnComponentParser : public ComponentParser {
	public:
		SpawnComponentParser();
		virtual ~SpawnComponentParser();
		ComponentFactory* parse(InputStream& in);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);

	protected:
		const DictionaryEntry dict_;
		enum { TYPE = sct_SPAWN, SUBTYPE = 0 };
	};

	extern _SeCoreExport const SpawnComponentParser parserSpawnComponent;
}

#endif
