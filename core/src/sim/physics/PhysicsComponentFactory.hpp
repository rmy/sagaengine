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


#ifndef PhysicsComponentFactory_hpp
#define PhysicsComponentFactory_hpp

#include "Move.hpp"
#include "sim_physics.hpp"
#include "../sim.hpp"
#include "../config/sim_config.hpp"
#include "comp/ComponentFactory.hpp"
#include "comp/Composite.hpp"
#include "comp/ComponentFactory.hpp"
#include "util/type/util_type.hpp"
#include "Move.hpp"

namespace se_core {
	class PhysicsComponentFactory : public ComponentFactory {
	public:
		PhysicsComponentFactory();
		Component* create(Composite* owner) const;
		void setPhysics(const char* name);
		void setMass(scale_t m);
		void setFriction(float linear, float angular, float bounceDecay);

	protected:
		se_core::Move move_;
		const se_core::Physics* physics_;
	};

}

#endif
