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

#include "sim_script.hpp"
#include "comp/Object.hpp"
#include "sim/sim.hpp"
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
	class _SeCoreExport Script : public Object {
	public:
		static const Script* lookup(const char* name) {
			return static_cast<const Script*>(_lookup(got_SCRIPT, name));
		}

		Script(const char* name);
		virtual ~Script();
		virtual ScriptData* init(const ScriptComponent& performer) const { return 0; }
		virtual void reinit(const ScriptComponent& performer, ScriptData* sd) const {}
		virtual void reinitPop(const ScriptComponent& performer, ScriptData* sd, const char* fromScript) const {}
		virtual void touched(const ScriptComponent& performer, ScriptData* sd, void* param) const {}
		void release(ScriptData* &sd) const { delete sd; sd = 0; }
		virtual void nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, ActionAndParameter& out) const;
		virtual const Action* nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, Parameter& out) const { return 0; }

		enum { fb_SPEECH_FINISHED };
		virtual void feedbackEvent(const ScriptComponent& performer, ScriptData* sd, int type) const {}

		/**
		 *	Should return false if the script should
		 *	be popped before any is pushed on top of it.
		 */
		virtual bool isStacker() const { return true; }
		virtual bool isTransparent() const { return false; }

	private:
		String nameString_; // For proper destruction of name strings
	};

}

#endif
