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


#ifndef o3d_event_RenderEventListener_hpp
#define o3d_event_RenderEventListener_hpp


namespace se_ogre {
	class _SeOgreExport RenderEventListener {
	public:
		RenderEventListener() : isActive_(false) {}
		void setActive(bool state);
		bool isActive() { return isActive_; }
		virtual bool initEngineEvent() { return true; }
		virtual bool initGameEvent() { return true; }
		virtual bool initLevelEvent() { return true; }
		virtual void cleanupLevelEvent() {}
		virtual void cleanupGameEvent() {}
		virtual void cleanupEngineEvent() {}
		virtual void updateLights(float renderClock) {}
		virtual void updateMaterials(float renderClock) {}
		virtual void updateImpostors(float renderClock) {}

	protected:
		bool isActive_;
	};
}

#endif
