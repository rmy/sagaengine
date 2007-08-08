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


#ifndef ScriptComponentFactory_hpp
#define ScriptComponentFactory_hpp

#include "../sim.hpp"
#include "comp/ComponentFactory.hpp"
#include "../action/Action.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "comp/Composite.hpp"
#include "comp/ComponentFactory.hpp"
#include "util/type/util_type.hpp"
#include "sim/stat/PropertyHashTable.hpp"

namespace se_core {
	class ScriptComponentFactory : public ComponentFactory {
	public:
		ScriptComponentFactory();
		Component* create(Composite* owner) const;

		void setScript(const char* name);

	protected:
		static const int MAX_SCRIPTS = 4;
		int scriptCount_;
		const se_core::Script* scripts_[ MAX_SCRIPTS ];
	};

}

#endif
