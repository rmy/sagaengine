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
		if(state == true) return;
		isActive_ = true;

		// Setting to active??
		if(isActive_) {
			// Start script
			if(script()) {
				// Init (or reinit) scripts data block
				script()->reinit(*this, scriptData());
				//scriptData_ = script()->init(*this);
				// Start script in all action channels
				for(int i = 0; i < CHANNEL_COUNT; ++i) {
					nextScriptAction(i);
				}
			}
		}
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
				handler_->cameraEnteredAreaEvent(*this, *nextPos().area());
			}
			return true;
		}
		return false;
	}

}