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


#ifndef engine_script_ShowingCutscene_hpp
#define engine_script_ShowingCutscene_hpp

#include "sim_script.hpp"
#include "../stat/sim_stat.hpp"
#include "../thing/sim_thing.hpp"
#include "comp/list/CompositeList.hpp"

namespace se_core {
	class _SeCoreExport ShowingCutscene {
	public:
		ShowingCutscene();
		virtual ~ShowingCutscene();
		void addMember(Composite& a);
		void removeMember(Composite& a);
		void freeMembers();
		void setCutscene(Cutscene* cutscene) { cutscene_ = cutscene; }
		Cutscene* cutscene() { return cutscene_; }

	private:
		Cutscene* cutscene_;
		CompositeList* members_;
	};
}

#endif
