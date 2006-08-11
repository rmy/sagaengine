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


#ifndef game_Forward_hpp
#define game_Forward_hpp

#include "sim/action/Action.hpp"

namespace logic {
	class Forward : public se_core::Action {
	public:
		Forward() : se_core::Action("Forward") {}

		short duration(se_core::Actor& performer, se_core::Parameter& parameter) const { return 1; }
		bool isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const { return false; }
		bool isRepeating(long when, se_core::Actor &performer, se_core::Parameter& parameter) const { return true; }
		void perform(long when, se_core::Actor& performer, se_core::Parameter& parameter) const;

		void param(float speed, se_core::Parameter& out) const;

	private:
		struct Param {
			Param(scale_t s)
				: speed_(s) {
			}
			scale_t speed_;
		};
	};


	extern const Forward actionForward;
}

#endif
