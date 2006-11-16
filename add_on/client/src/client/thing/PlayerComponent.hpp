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

#include "sim/SimComponent.hpp"
#include "sim/SimComposite.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/action/ActionComponent.hpp"
#include "sim/action/Action.hpp"
#include "sim/action/ActionAndParameter.hpp"
#include "sim/action/sim_action.hpp"
#include "sim/thing/sim_thing.hpp"
#include "util/type/all.hpp"

namespace se_client {
	class _SeCoreExport PlayerComponent : public se_core::SimComponent {
	public:
		PlayerComponent(se_core::SimComposite* owner);
		virtual ~PlayerComponent();

		void setActive(bool state);
		void areaChanged(se_core::SimComposite* newArea, se_core::SimComposite* oldArea);
		void planAction(short channel, const se_core::Action& action, const se_core::Parameter* parameter = 0) const;

		const se_core::ActionAndParameter& defaultMovementAction() const { return defaultMovementAction_; }
		void setDefaultMovementAction(const se_core::Action& action, const se_core::Parameter* parameter = 0) const { 
			LogMsg(action.name());
			defaultMovementAction_.setAction(action);
			if(parameter) {
				defaultMovementAction_.copyParameter(*parameter);
			}
		}
		const se_core::ActionAndParameter& defaultTurnAction() const { return defaultTurnAction_; }
		void setDefaultTurnAction(const se_core::Action& action, const se_core::Parameter* parameter = 0) const {
			defaultTurnAction_.setAction(action);
			if(parameter) {
				defaultTurnAction_.copyParameter(*parameter);
			}
		}

		/**
		 * Plan to perform the default movement action.
		 *
		 * Plans the default movement action when the action that is presently
		 * in the ActionQueue is completed, if no other action is planned
		 * for that channel. The default movement Action is typically Walk
		 * or Stand. This method allows the player character to resume movement
		 * after the player has pushed a buttton to do something else.
		 *
		 * If you want to force planning this action, do a
		 * clearPlannedAction(...) first.
		 *
		 * @see clearPlannedAction
		 */
		void planDefaultMovementAction() const;
		void performDefaultMovementAction() const;


		static PlayerComponent* get(se_core::SimComposite& composite) {
			PlayerComponent* c = static_cast<PlayerComponent*>(composite.component(se_core::sct_PLAYER));
			return c;
		}

	protected:
		// Sibling shortcut
		se_core::ActionComponent* actionComponent_;
		se_core::PosComponent* posComponent_;

		se_core::ViewPoint lastEntrance_;

		mutable se_core::ActionAndParameter defaultMovementAction_;
		mutable se_core::ActionAndParameter defaultTurnAction_;
	};

}

#endif
