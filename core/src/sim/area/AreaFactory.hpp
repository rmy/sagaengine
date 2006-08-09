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

namespace se_core {

	class AreaFactory {
	public:
		AreaFactory(String* name);
		virtual ~AreaFactory();
		const char* name() const;
		virtual Area* create(String* name, int pageX, int pageY, int pageZ) const = 0;
		virtual void release(Area* a) const;

	protected:
		String* name_;
	};

}

#endif