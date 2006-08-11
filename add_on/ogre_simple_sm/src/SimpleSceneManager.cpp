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


#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include "SimpleSceneManager.hpp"

namespace se_ogre {

	SimpleSceneManager
	::SimpleSceneManager() {
		areaSceneNode_ = getRootSceneNode()->createChildSceneNode("area");
	}


	SimpleSceneManager
	::~SimpleSceneManager() {
	}


	void SimpleSceneManager
	::setWorldGeometry(const Ogre::String& name) {
		Ogre::String mesh = name + ".mesh";
		Ogre::Entity* e = createEntity(name, mesh);
		areaSceneNode_->attachObject(e);
	}


}