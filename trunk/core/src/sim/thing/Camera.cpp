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


#include "Camera.hpp"
#include "CameraHandler.hpp"
#include "Actor.hpp"
#include "../action/sim_action.hpp"
#include "../action/Action.hpp"
#include "../script/Script.hpp"
#include "../area/Area.hpp"
#include "../../util/error/Log.hpp"


namespace se_core {

	Camera
	::Camera(const char* name) : Actor(name), handler_(0) {
	}


	Camera
	::~Camera() {
	}


	void Camera
	::setActive(bool state) {
		isActive_ = true;
		/*
		if(state == false) return;
		isActive_ = state;
		if(isActive_) {
			if(script_) {
				for(int i = 0; i < CHANNEL_COUNT; ++i) {
					const Action* a = const_cast<Script*>(script_)->nextAction(*this, i);
					if(a) planActionWithoutSideEffects(a);
				}
			}
		}
		*/
	}


	void Camera
	::leaveCurrentArea() {
		if(!area()) return;
		Actor::leaveCurrentArea();
		if(handler_) handler_->cameraLeftAreaEvent(*this, *area());
	}


	bool Camera
	::changeArea() {
		if(Actor::changeArea()) {
			if(handler_) {
				WasHere();
				handler_->cameraEnteredAreaEvent(*this, *nextPos().area());
			}
			WasHere();
			return true;
		}
		return false;
	}

}
