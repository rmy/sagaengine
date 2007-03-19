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


#ifndef CutsceneAreaComponent_hpp
#define CutsceneAreaComponent_hpp

#include "sim/sim.hpp"
#include "sim/AreaComponent.hpp"


namespace se_core {
	class _SeCoreExport CutsceneAreaComponent : public AreaComponent {
	public:
		typedef Ptr<CutsceneAreaComponent, sct_CUTSCENE> Ptr;
		
		/** Constructor.
		 */
		CutsceneAreaComponent(SimComposite* owner, const SimComponentFactory* factory = 0);
		~CutsceneAreaComponent();
		void setActive(bool state);

		const char* name() { return "CutsceneArea"; }

		//void enter(Actor& performer);

	protected:
		friend class CutsceneAreaComponentFactory;
	};
}

#endif
