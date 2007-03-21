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


#ifndef engine_hpp
#define engine_hpp

namespace se_core {
	class InitListener;
	class InitListeners;
	class SimEngine;
	class SimListener;
	class SimListeners;
	class SimObject;

	class ComponentFactory;
	class Component;
	class NodeComponent;

	class CompositeFactory;
	class Composite;
	
	class SimCompositeManager;

	enum SimObjectType {
		got_SIM_OBJECT, got_SIGNAL, got_ACTION, got_PHYSICS, got_SIM_COMPOSITE, got_AREA,
		got_TRIMAP_AREA, got_HEIGHTMAP_AREA, got_TERRAIN,
		got_POS_NODE, got_THING, got_ACTOR, got_CAMERA,
		got_PLAYER, got_SCRIPT, got_CUTSCENE,
		got_QUEST_GOAL, got_NUMBER_OF_TYPES, got_THING_COLLIDE,
		got_USER_DEFINED
	};

	enum SimComponentType { 
		sct_UNDEFINED, 
		sct_BLOB, 
		sct_POS, 
		sct_PHYSICS, 
		sct_COLLISION, 
		sct_ACTION, 
		sct_SCRIPT, 
		sct_SPAWN, 
		sct_STAT, 
		sct_SIGNAL,
		sct_RENDER, 
		sct_SOUND, 
		sct_ODE, 
		sct_PLAYER, 
		sct_CAMERA, 
		sct_PLAYER_LOGIC,
		sct_LOGIC,
		sct_UI,
		sct_CUTSCENE,
		sct_EDITOR,
		sct_NODE
	};

}

#endif
