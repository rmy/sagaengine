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


#ifndef CustomScript_hpp
#define CustomScript_hpp

#include "Script.hpp"
#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport CustomScript : public Script {
	public:
		CustomScript(char* name);
		ScriptData* init(const Actor& performer) const;
		const Action* nextAction(const Actor& performer, int channel, ScriptData* sd) const;
		virtual const Action* nextDirection(const Actor& performer, ScriptData* sd) const;
		virtual const Action* nextMovement(const Actor& performer, ScriptData* sd) const;
		virtual const Action* nextExtra(const Actor& performer, ScriptData* sd) const;
	private:
	};

}

#endif
