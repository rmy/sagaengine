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


#ifndef O3dThingComponent_hpp
#define O3dThingComponent_hpp

#include "O3dPre.hpp"
#include "ThingMOList.hpp"
#include "../O3dNodeComponent.hpp"
#include "o3d_thing.hpp"
#include "util/task/Task.hpp"
#include "util/vecmath/ViewPoint.hpp"

namespace se_ogre {
	class _SeOgreExport O3dThingComponent  : public O3dNodeComponent, public se_core::Task {
	public:
		O3dThingComponent(se_core::Composite* owner);
		~O3dThingComponent();

		static O3dThingComponent* get(se_core::Composite* composite) {
			Assert(composite);
			O3dThingComponent* c = static_cast<O3dThingComponent*>(composite->component(se_core::sct_RENDER));
			return c;
		}

		static O3dThingComponent* get(se_core::Composite& composite) {
			O3dThingComponent* c = static_cast<O3dThingComponent*>(composite.component(se_core::sct_RENDER));
			return c;
		}

		static O3dThingComponent* get(se_core::Component& component) {
			O3dThingComponent* c = static_cast<O3dThingComponent*>(component.owner()->component(se_core::sct_RENDER));
			return c;
		}

		void init();
		void cleanup();
		void perform();
		void clear();

		void setActive(bool state);
		void setVisible(bool state);

		void move(long when, float stepDelta, float timeSinceLastFrame);
		void setParentNode(Ogre::SceneNode* sn);
		void resetParentNode();

		void add(ThingMO& tmo);
		void remove(ThingMO& tmo);

		void parentChanged(se_core::Composite* newArea, se_core::Composite* oldArea) {}
		void zoneChanged(int type, se_core::Composite* newArea, se_core::Composite* oldArea);

	protected:
		Ogre::SceneNode* parentNode_;
		se_core::ViewPoint last_;
		bool isVisible_;
		bool isInitialized_;

		ThingMOList::iterator_type firstThingMO_;
	};

}

#endif
