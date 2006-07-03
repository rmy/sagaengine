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


#ifndef Action_hpp
#define Action_hpp

#include "sim_action.hpp"
#include "../SimObject.hpp"
#include "../thing/sim_thing.hpp"

namespace se_core {

	/**
	 * Base class for actions.
	 * Actions are the atoms of game-character behaviour in SagaEngine.
	 * They define what a character can do.
	 */
	class Action : public SimObject {
	public:
		Action(const char* name);
		virtual short duration(Actor& performer, Parameter& parameter) const = 0;
		virtual bool isRepeating(long when, Actor &performer, Parameter& parameter) const = 0;
		virtual bool isContinuing(Actor &performer, Parameter& parameter) const { return false; }
		virtual void prepare(Actor &performer, Parameter& parameter) const {}
		virtual void disrupt(Actor &performer, Parameter& parameter) const {}
		virtual void perform(long when, Actor &performer, Parameter& parameter) const = 0;
	};

}

#endif
