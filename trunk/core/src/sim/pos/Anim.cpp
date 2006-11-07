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


#include "Anim.hpp"

namespace se_core {
	int Anim::MOVEMENT_MODE_COUNT = 12;

	Anim
	::Anim() 
		: movementMode_(0),
		  movementStartedWhen_(0),
		  pos_(0),
		  weight_(1),
		  speed_(1),
		  startPos_(0) {
	}


	void Anim
	::setAnim(const Anim& original) {
		*this = original;
	}


	void Anim
	::setMovementMode(short mode) {
		movementMode_ = mode;
	}


	void Anim
	::setMovementMode(long when, short mode) {
		movementStartedWhen_ = when;
		movementMode_ = mode;
	}


	short Anim
	::movementMode() const {
		return movementMode_;
	}

}
