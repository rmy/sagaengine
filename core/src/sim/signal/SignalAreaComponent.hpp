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


#ifndef engine_physics_SignalAreaComponent_hpp
#define engine_physics_SignalAreaComponent_hpp

#include "../AreaComponent.hpp"
#include "../react/CollisionGrid.hpp"
#include "../react/sim_react.hpp"
#include "../area/Area.hpp"
#include "../pos/PosComponent.hpp"

namespace se_core {
	class _SeCoreExport SignalAreaComponent : public AreaComponent {
	public:
		/** Constructor.
		 */
		SignalAreaComponent(SimComposite* owner);
		~SignalAreaComponent();

		const char* name() { return "SignalArea"; }

		void setActive(bool state);
		void setSignalActive(int id, bool state);

		void propagate();
		void propagate(int id);

	private:
		static const int MAX_SIGNALS = 32;
		unsigned long changed_;
		int activeSignals_[MAX_SIGNALS];
	};
}

#endif
