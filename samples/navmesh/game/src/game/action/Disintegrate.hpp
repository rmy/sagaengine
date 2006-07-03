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


#ifndef Disintegrate_hpp
#define Disintegrate_hpp

#include "sim/action/Action.hpp"

namespace se_core {

	class Disintegrate : public Action {
	public:
		Disintegrate() : Action("disintegrate") {}

		short duration(Actor& performer, Parameter& parameter) const;

		short channel() const {
			return CHANNEL_MOVEMENT;
		}

		bool isRepeating(long when, Actor &performer, Parameter& parameter) const {
			return false;
		}

		void perform(long when, Actor& performer, Parameter& parameter) const;

		const Disintegrate& param(short millis, Parameter& out) const;

	private:

		struct Param {
			short millis_;
		};
	};

	extern const Disintegrate actionDisintegrate;
}


#endif
