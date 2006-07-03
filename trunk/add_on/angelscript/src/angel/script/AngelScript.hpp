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


#ifndef angel_script_AngelScript_hpp
#define angel_script_AngelScript_hpp

#include "sim/action/Action.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/script/Script.hpp"
#include <angelscript.h>


namespace se_core {


	class AngelScript : public Script {
	public:
		AngelScript(String* name);
		const Action* nextAction(const Actor& performer, int channel, ScriptData* sd, Parameter& out) const;
		ScriptData* init(const Actor& performer) const;
		void reinit(const Actor& performer, ScriptData* sd) const;

	private:
		int initFuncId_, reinitFuncId_, transitionFuncId_, sequenceFuncId_;
	};
}

#endif
