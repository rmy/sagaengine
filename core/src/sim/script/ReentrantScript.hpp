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


#ifndef engine_script_ReentrantScript_hpp
#define engine_script_ReentrantScript_hpp

#include "Script.hpp"
#include "../action/sim_action.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport ReentrantData : public ScriptData {
	public:
		ReentrantData() : rp(0) {}
		void restartScript() { rp = 0; }
		long rp; // Reentrance pointer
	};


	class _SeCoreExport ReentrantScript : public Script {
	public:
		ReentrantScript(const char* name, int channel = 0);
		const Action* nextAction(const ScriptComponent& performer, int channel, ScriptData* sd, Parameter& out) const;

		virtual ScriptData* init(const ScriptComponent& performer) const = 0;

		virtual const Action* sequence(const ScriptComponent& performer, ReentrantData& _, Parameter& out) const = 0;
		virtual const Action* transition(const ScriptComponent& performer, ReentrantData& _, Parameter& out) const = 0;

	private:
		int channel_;
	};
}

#define REENTRANT(var) Data& var = static_cast<Data&>(rd); switch(rd.rp)
#define START case 0:
//#define PERFORM(action) rd.rp = __LINE__; return &action; case __LINE__:
#define PERFORM(action, LABEL) rd.rp = LABEL; return &action; case LABEL:
#define RESTART rd.rp = 0; return 0;

#endif
