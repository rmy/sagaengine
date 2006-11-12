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


#ifndef O3dAreaComponent_hpp
#define O3dAreaComponent_hpp

#include "O3dPre.hpp"
#include "../thing/MultiO3dThingComponent.hpp"
#include "o3d_area.hpp"

namespace se_ogre {
	class _SeOgreExport O3dAreaComponent  : public se_core::SimComponent {
	public:
		O3dAreaComponent(se_core::SimComposite* owner);
		~O3dAreaComponent();
		void clear() { setVisible(false); }
		void setActive(bool state);
		void setVisible(bool state);

		void move(long when, float stepDelta, float timeSinceLastFrame);
		void addChild(O3dThingComponent& c);
		void removeChild(O3dThingComponent& c);

		void areaOffset(Ogre::Vector3& dest);

	protected:
		void init();
		void initThings();

		Ogre::StaticGeometry* compileStaticGeometry();

		friend class WorldManager;
		Ogre::SceneNode* node_;
		Ogre::StaticGeometry* staticGeometry_;
		Ogre::Vector3 offset_;
		bool shouldKeep_, isNew_;
		bool isInitialized_;
		bool isVisible_;

		MultiO3dThingComponent children_;
	};

}

#endif
