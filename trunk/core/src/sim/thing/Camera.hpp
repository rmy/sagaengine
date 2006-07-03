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


#ifndef Camera_hpp
#define Camera_hpp

#include "Actor.hpp"
#include "sim_thing.hpp"
#include "sim/stat/sim_stat.hpp"


namespace se_core {
	class Camera : public Actor {
	public:
		Camera(const char* name);
		virtual ~Camera();
		bool isType(enum SimObjectType type) const {
			if(type == got_CAMERA) return true;
			return Actor::isType(type);
		}
		virtual void setActive(bool state);
		bool isMover() { return false; }
		void setCameraHandler(CameraHandler* handler) { handler_ = handler; }
		void leaveCurrentArea();
		bool changeArea();

	private:
		CameraHandler* handler_;
	};

}

#endif
