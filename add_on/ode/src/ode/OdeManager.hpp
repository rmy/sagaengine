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


#ifndef OdeManager_hpp
#define OdeManager_hpp

#include "OdePre.hpp"

namespace se_ode {
	class _SeOdeExport OdeManager : public se_core::SimComponentManager {
	public:
		OdeManager();
		~OdeManager();

		void step(long when);
		dBodyID createBody();
		void destroyBody(dBodyID id);

		dSpaceID createSpace();
		void destroySpace(dSpaceID id);

		static OdeManager& singleton();

		void collision(dGeomID o1, dGeomID o2);
		void applyForces();
		void update();

	protected:
		dWorldID worldId_;
		dSpaceID spaceId_;
		dJointGroupID collisionJointGroup_;

		static const int MAX_CONTACTS = 32;
		int contactCount_, activeContactCount_;
		dContact contacts_[MAX_CONTACTS];
	};
}

#endif
