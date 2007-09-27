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


#include "InputHandler.hpp"
#include "InputManager.hpp"
#include "o3d/schema/use.hpp"


namespace se_ogre {

	InputHandler
	::InputHandler(const char* name) {
		name_ = name;
		O3dSchema::inputManager().add(this);
	}

	InputHandler
	::~InputHandler() {
		O3dSchema::inputManager().remove(this);
	}


	bool InputHandler
	::isShiftDown() const {
		return (O3dSchema::inputManager().isShiftDown());
	}

	bool InputHandler
	::isAltDown() const {
		return (O3dSchema::inputManager().isAltDown());
	}

	bool InputHandler
	::isControlDown() const {
		return (O3dSchema::inputManager().isControlDown());
	}

	bool InputHandler
	::isWinDown() const {
		return (O3dSchema::inputManager().isWinDown());
	}

	bool InputHandler
	::hasFocus() {
		return (O3dSchema::inputManager().active() == this);
	}

	void InputHandler
	::grabFocus() {
		if(hasFocus()) return;
		O3dSchema::inputManager().push(this);
	}


	void InputHandler
	::loseFocus() {
		AssertFatal(O3dSchema::inputManager().active() == this, "Hasn't got focus: " << name());
		O3dSchema::inputManager().pop();
	}

	InputHandler* InputHandler
	::below() {
		return O3dSchema::inputManager().below();
	}

	const char* InputHandler
	::name() {
		return name_;
	}

}

