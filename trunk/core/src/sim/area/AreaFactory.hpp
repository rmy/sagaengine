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


#ifndef AreaFactory_hpp
#define AreaFactory_hpp

#include "sim_area.hpp"
#include "util/type/util_type.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../SimCompositeFactory.hpp"
#include "../SimComponentFactory.hpp"
#include "../SimComposite.hpp"

namespace se_core {

	class _SeCoreExport AreaFactory : public SimCompositeFactory {
	public:
		AreaFactory(String* name);
		virtual ~AreaFactory();
		virtual Area* create(String* name, int pageX, int pageY, int pageZ) const = 0;

		void createGenericComponents(SimComposite* owner) const;
		
		static void addGenericComponent(SimComponentFactory* f) {
			genericComponents_[ genericComponentCount_++ ] = f;
		}

	private:
		static const int MAX_GENERIC_COMPONENTS = 4;
		static int genericComponentCount_;
		static SimComponentFactory* genericComponents_[MAX_GENERIC_COMPONENTS];
	};

}

#endif
