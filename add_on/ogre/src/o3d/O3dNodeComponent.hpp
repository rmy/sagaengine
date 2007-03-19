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


#ifndef O3dNodeComponent_hpp
#define O3dNodeComponent_hpp

#include "O3dPre.hpp"

namespace se_ogre {
	class _SeOgreExport O3dNodeComponent  : public se_core::SimNodeComponent {
	public:
		O3dNodeComponent(enum se_core::SimComponentType type, se_core::Composite* owner);
		~O3dNodeComponent();

		static O3dNodeComponent* get(se_core::Composite* composite) {
			Assert(composite);
			O3dNodeComponent* c = static_cast<O3dNodeComponent*>(composite->component(se_core::sct_RENDER));
			return c;
		}

		static O3dNodeComponent* get(se_core::Composite& composite) {
			O3dNodeComponent* c = static_cast<O3dNodeComponent*>(composite.component(se_core::sct_RENDER));
			return c;
		}

		static O3dNodeComponent* get(se_core::SimComponent& component) {
			O3dNodeComponent* c = static_cast<O3dNodeComponent*>(component.owner()->component(se_core::sct_RENDER));
			return c;
		}

		Ogre::SceneNode* node() {
			return node_;
		}

	protected:
		Ogre::SceneNode* node_;
	};
}

#endif
