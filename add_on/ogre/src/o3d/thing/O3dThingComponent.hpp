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

namespace se_ogre {
	class _SeOgreExport O3dThingComponent  : public se_core::SimComponent {
	public:
		O3dThingComponent(se_core::Actor* owner);
		~O3dThingComponent();
		void setActive(bool state);
		void setVisible(bool state);

		void move(long when, float stepDelta, float timeSinceLastFrame);
		void setParentNode(Ogre::SceneNode* sn);

		void add(ThingMO& tmo);
		void remove(ThingMO& tmo);


	protected:
		Ogre::SceneNode* parentNode_;
		Ogre::SceneNode* node_;
		se_core::ViewPoint last_;
		bool isVisible_;

		ThingMOList::iterator_type firstThingMO_;
	};

}

#endif
