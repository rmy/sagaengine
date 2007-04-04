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


#include "O3dPre.hpp"
#include "O3dNodeComponent.hpp"
#include "schema/O3dSchema.hpp"
#include <OgreSceneManager.h>


using namespace se_core;
using namespace se_client;

namespace se_ogre {

	O3dNodeComponent
	::O3dNodeComponent(int type, Composite* owner)
			: NodeComponent(type, owner), node_(0) {
		node_ = O3dSchema::sceneManager->createSceneNode();
	}

	O3dNodeComponent
	::~O3dNodeComponent() {
		node_->removeAndDestroyAllChildren();
		O3dSchema::sceneManager->destroySceneNode(node_->getName());
		node_ = 0;
	}

}
