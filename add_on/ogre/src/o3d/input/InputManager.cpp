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


#include "InputManager.hpp"
#include "InputHandler.hpp"
#include "util/error/Log.hpp"


namespace se_ogre {
	InputManager
	::InputManager() : handlerCount_(0), handlerStackPos_(-1) {
		for(int i = 0; i < MODIFIER_COUNT; ++i) {
			isModifierDown_[i] = false;
		}
	}

	InputManager
	::~InputManager() {
	}


	InputHandler* InputManager
	::active() {
		if(handlerStackPos_ < 0) {
			//LogFatal("No active input control");
			return 0;
		}
		return handlerStack_[ handlerStackPos_ ];
	}


	InputHandler* InputManager
	::below() {
		if(handlerStackPos_ < 1) {
			return 0;
		}
		return handlerStack_[ handlerStackPos_ - 1 ];
	}


	void InputManager
	::push(InputHandler* handler) {
		Assert(handlerStackPos_ + 1 < MAX_STACK_SIZE);
		handlerStack_[ ++handlerStackPos_ ] = handler;
	}


	void InputManager
	::push(const char* name) {
		InputHandler* h = handler(name);
		Assert(h && "No input handler with that name");
		if(handlerStackPos_ >= 0) {
			handlerStack_[ handlerStackPos_ ]->lostFocusEvent();
		}
		handlerStack_[ ++handlerStackPos_ ] = h;
		handlerStack_[ handlerStackPos_ ]->grabbedFocusEvent();
	}


	void InputManager
	::pop() {
		Assert(handlerStackPos_ >= 0 && "Stack already empty");
		handlerStack_[ handlerStackPos_ ]->lostFocusEvent();
		--handlerStackPos_;
		if(handlerStackPos_ >= 0) {
			handlerStack_[ handlerStackPos_ ]->grabbedFocusEvent();
		}
	}


	void InputManager
	::add(InputHandler* handler) {
		Assert(!hasHandler(handler->name()));
		Assert(handlerCount_ < MAX_HANDLER_COUNT );
		handlers_[ handlerCount_++ ] = handler;
	}


	void InputManager
	::remove(InputHandler* handler) {
		for(int i = 0; i < handlerCount_; ++i) {
			if(handlers_[ i ] == handler) {
				handlers_[ i ] = handlers_[ --handlerCount_ ];
				break;
			}
		}
	}


	InputHandler* InputManager
	::handler(const char* name) {
		for(int i = 0; i < handlerCount_; ++i) {
			if(strcmp(name, handlers_[ i ]->name()) == 0) {
				return handlers_[ i ];
			}
		}
		return 0;
	}


	bool InputManager
	::hasHandler(const char* name) {
		return (handler(name) != 0);
	}
}

