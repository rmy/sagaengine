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


#ifndef Script_hpp
#define Script_hpp

#include "../SimObject.hpp"
#include "../action/sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	/** Base class for script data. */
	class _SeCoreExport ScriptData {
	public:
		ScriptData();
		virtual ~ScriptData();

	public:
		DebugExec(static int refCount);
	};

	/**
	 * Base class for scripts.
	 * A script analyses the sitiation of a game actor and plans the next
	 * action. A script does not change the state of the performer in any
	 * way, except for what is needed to communicate with actions.
	 */
	class _SeCoreExport Script : public SimObject {
	public:
		Script(const char* name);
		virtual ~Script();
		virtual ScriptData* init(const Actor& performer) const { return 0; }
		virtual void reinit(const Actor& performer, ScriptData* sd) const {}
		void release(ScriptData* &sd) const { delete sd; sd = 0; }
		virtual const Action* nextAction(const Actor& performer, int channel, ScriptData* sd, Parameter& out) const = 0;
		void trackUserFeedback() const;

		/**
		 *	Should return false if the script should
		 *	be popped before any is pushed on top of it.
		 */
		virtual bool isStacker() const { return true; }

	private:
		String nameString_; // For proper destruction of name strings
	};

}

#endif
