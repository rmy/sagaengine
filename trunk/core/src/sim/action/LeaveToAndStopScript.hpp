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


#ifndef LeaveToAndStopScript_hpp
#define LeaveToAndStopScript_hpp

#include "sim/action/Action.hpp"

namespace se_core {
	class _SeCoreExport LeaveToAndStopScript : public Action {
	public:
		LeaveToAndStopScript()  : Action("leave_to") {}
		short duration(Actor& performer, Parameter& parameter) const { return 1; }
		bool isRepeating(long when, Actor &performer, Parameter& parameter) const { return false; }
		void perform(long when, Actor& performer, Parameter& parameter) const;

	private:
		void param(const char* areaName, short entranceId, Parameter& out);

		struct Param {
			const char* areaName_;
			int entranceId_;
		};
	};


	extern _SeCoreExport const LeaveToAndStopScript actionLeaveTo;
}

#endif
