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


#ifndef ActionFeed_hpp
#define ActionFeed_hpp

#include "sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	/**
	 * Base class for scripts.
	 * A script analyses the sitiation of a game actor and plans the next
	 * action. A script does not change the state of the performer in any
	 * way, except for what is needed to communicate with actions.
	 */
	class _SeCoreExport ActionFeed {
	public:
		virtual void nextAction(const ActionComponent& performer, int channel, ActionAndParameter& out) = 0;
	};

}

#endif
