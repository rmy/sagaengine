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


#ifndef SimComponent_hpp
#define SimComponent_hpp

#include "sim.hpp"
#include "thing/sim_thing.hpp"
#include "area/sim_area.hpp"
#include "util/error/Log.hpp"
#include "util/type/String.hpp"
#include "comp/Component.hpp"

namespace se_core {
	/**
	 * Base class for functionality composites.
	 */
	class _SeCoreExport SimComponent : public Component {
	public:
		/**
		 * Construct unnamed SimComponent.
		 *
		 * @param type The type of SimComponent (gct_ACTOR, gct_PHYSICS, etc)
		 */
		SimComponent(int type, Composite* owner, const SimComponentFactory* factory = 0);


		Actor* toActor();
		const Actor* toActor() const;

		inline Area* toArea() {
			return reinterpret_cast<Area*>(owner_);
		}
		inline const Area* toArea() const {
			return reinterpret_cast<Area*>(owner_);
		}

	};


}

#endif
