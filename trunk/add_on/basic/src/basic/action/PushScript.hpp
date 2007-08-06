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


#ifndef PushScript_hpp
#define PushScript_hpp

#include "sim/action/Action.hpp"

namespace se_basic {
	class _SeBasicExport PushScript : public se_core::Action {
	public:
		PushScript() : se_core::Action("push_script") {}
		short duration(se_core::ActionComponent& performer, se_core::Parameter& parameter) const { return 1; }
		bool isRepeating(long when, se_core::ActionComponent &performer, se_core::Parameter& parameter) const { return false; }

		void perform(long when, se_core::ActionComponent& performer, se_core::Parameter& parameter) const;
		const PushScript& param(const char* script, se_core::Parameter& out) const;
		struct Param {
			Param(const char* script)
				: script_(script) {
			}
			const char* script_;
		};
	};


	extern _SeBasicExport const PushScript actionPushScript;
}

#endif
