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


#ifndef ZoneComponentFactory_hpp
#define ZoneComponentFactory_hpp

#include "comp/comp.hpp"
#include "comp/ComponentFactory.hpp"
#include "util/type/util_type.hpp"
#include "sim_zone.hpp"


namespace se_core {
	class _SeCoreExport ZoneAreaComponentFactory : public ComponentFactory {
	public:
		ZoneAreaComponentFactory();
		Component* create(Composite* owner) const;

		void setExits(Exit* exits, int count);


	protected:
		int exitCount_;
		Exit* exits_;

	};

}

#endif
