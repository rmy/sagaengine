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


#ifndef Idle_hpp
#define Idle_hpp

#include "Action.hpp"

namespace se_core {

	class _SeCoreExport Idle : public Action {
	public:
		Idle() : Action("idle") {}
		short duration(ActionComponent& performer, Parameter& parameter) const { return 1; }
		bool isRepeating(long when, ActionComponent &performer, Parameter& parameter) const {
			return false;
		}
		void perform(long when, ActionComponent& performer, Parameter& parameter) const {}
	};

	extern _SeCoreExport const Idle actionIdle;

}

#endif
