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


#ifndef PlayerComponent_hpp
#define PlayerComponent_hpp

#include "comp/Component.hpp"
#include "comp/Composite.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/action/Action.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/sim_action.hpp"
#include "sim/thing/sim_thing.hpp"
#include "util/type/all.hpp"

namespace se_client {
	class _SeCoreExport PlayerComponent : public se_core::Component {
	public:
		typedef se_core::Component::ComponentPtr<PlayerComponent, se_core::sct_PLAYER> Ptr;

		PlayerComponent(se_core::Composite* owner);
		virtual ~PlayerComponent();
		virtual void release() {
			// Has no factory.
			delete this;
		}

		void setActive(bool state);
		void zoneChanged(int zoneType, se_core::Composite* newArea, se_core::Composite* oldArea);

		bool die(bool didWin);
		bool didWin() { return didWin_; }
		long deathAge();
		bool isControlsActive() const {
			return isControlsActive_;
		}
		void setControlsActive(bool f) {
			isControlsActive_ = f;
		}


	protected:
		// Sibling shortcut
		bool didWin_, isControlsActive_;
		se_core::ActionComponent* actionComponent_;
		se_core::PosComponent* posComponent_;

		se_core::ViewPoint lastEntrance_;

		mutable se_core::ActionAndParameter defaultMovementAction_;
		mutable se_core::ActionAndParameter defaultTurnAction_;
		long deadWhen_;
	};

}

#endif
