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


#ifndef SpawnComponentFactory_hpp
#define SpawnComponentFactory_hpp

#include "../sim.hpp"
#include "comp/ComponentFactory.hpp"
#include "../config/sim_config.hpp"
#include "comp/Composite.hpp"
#include "comp/ComponentFactory.hpp"
#include "util/type/util_type.hpp"
#include "util/vecmath/util_vecmath.hpp"

namespace se_core {
	class SpawnComponentFactory : public ComponentFactory {
	public:
		SpawnComponentFactory();
		void setAbilities(short speed, short attack, short defence, short level);

		Component* create(Composite* owner) const;
		/**
		 * Copy spawn points.
		 */
		void setSpawnPoints(int count, se_core::ViewPoint* const* spawnPoints);


	protected:
		int spawnPointCount_;
		se_core::ViewPoint** spawnPoints_;
	};

}

#endif
