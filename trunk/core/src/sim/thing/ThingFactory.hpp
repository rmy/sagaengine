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


#ifndef ThingFactory_hpp
#define ThingFactory_hpp

#include "sim_thing.hpp"
#include "util/type/util_type.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"
#include "../script/sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../SimCompositeFactory.hpp"

namespace se_core {

	class _SeCoreExport ThingFactory : public SimCompositeFactory {
	public:
		ThingFactory(short type, String* name)
			: SimCompositeFactory(type, st_THING, name) {
		}
	};
	/*
	public:
		ThingFactory(short type, String* name);
		virtual ~ThingFactory();
		const char* name() const;
		virtual Thing* create() const = 0;
		virtual void release(Thing* t);

		void setSingleValue(short id, short value);
		short singleValue(short id) { return singleValues_[id]; }
		MultiSimObject& multiSimObject(int type);
		String& attribute(short type);

	protected:
		String* name_;
		short type_;
		short maxSingleValues_, maxMultiSimObjects_, maxAttributes_;
		short* singleValues_;
		MultiSimObject** multiSimObjects_;
		String* attributes_;
	};
	*/

}

#endif
