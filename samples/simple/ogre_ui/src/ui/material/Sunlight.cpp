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


#include "Sunlight.hpp"
#include "OgreUiPre.hpp"
#include "o3d/schema/O3dSchema.hpp"
#include "o3d/RenderEngine.hpp"
#include <OgreSceneManager.h>

using namespace se_core;
using namespace se_ogre;

namespace ui {

	Sunlight
	::Sunlight() {
		setActive(true);
	}


	void Sunlight
	::init() {
		O3dSchema::sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
		light_ = O3dSchema::sceneManager->createLight("MainLight");
		light_->setType(Ogre::Light::LT_DIRECTIONAL);
		light_->setDiffuseColour(Ogre::ColourValue(1.0, 0.8, 0.5));
		light_->setSpecularColour(Ogre::ColourValue(0.1, 0.1, 0.1));
		light_->setCastShadows(true);
		light_->setDirection(-1, -1, -1);
	}

}
