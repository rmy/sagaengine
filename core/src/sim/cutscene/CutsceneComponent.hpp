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


#ifndef CutsceneComponent_hpp
#define CutsceneComponent_hpp

#include "sim/sim.hpp"
#include "sim/AreaChildComponent.hpp"

namespace se_core {
	class _SeCoreExport CutsceneComponent : public AreaChildComponent {
	public:
		typedef Ptr<CutsceneComponent, sct_CUTSCENE> Ptr;

		CutsceneComponent(Composite* owner, const SimComponentFactory* factory = 0);
		virtual ~CutsceneComponent();
		const char* name() { return "Cutscene"; }

		/*
		Cutscene* findRunnableCutscene(Actor& actor) { return 0; }

		///////////////////////////////////////
		void setShowingCutscene(ShowingCutscene* sc, const Script* script);
		ShowingCutscene* showingCutscene() { return showingCutscene_; }

		void removeFromShowingCutscene();
		void stopShowingCutscene();

		MultiSimObject& cutscenes() { return cutscenes_; }
		MultiSimObject& cutsceneMemberships() { return cutsceneMemberships_; }
		MultiSimObject& questGoals() { return questGoals_; }

	protected:
		friend class ShowingCutscene;
		mutable ActionAndParameter defaultAction_;
		*/

		/** ShowingCutscene uses this to free members. */
		//void setNoCutsceneShowing();

	/*
	protected:
		// Group shorts for compiler
		ShowingCutscene* showingCutscene_;

		MultiSimObject cutscenes_;
		MultiSimObject cutsceneMemberships_;
		MultiSimObject questGoals_;

	*/


	protected:
		friend class CutsceneComponentFactory;

		void cleanup();
	};


}

#endif
