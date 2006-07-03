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


#ifndef Player_hpp
#define Player_hpp

#include "Camera.hpp"
#include "../stat/sim_stat.hpp"
#include "../stat/Coor.hpp"

namespace se_core {
	class Player : public Camera {
	public:
		Player(const char* name);
		virtual ~Player();
		bool isType(enum SimObjectType type) const {
			if(type == got_PLAYER) return true;
			return Camera::isType(type);
		}
		MultiSimObject& multiSimObject(short type);
		const MultiSimObject& multiSimObject(short type) const;
		ReportingMultiSimObject& reportingMultiSimObject(short type);
		void scheduleForDestruction();

		/**
		 * Player objects are not really scheduled for destruction
		 * on a scheduleForDestruction call, as it is assumed that
		 * they are not to be destroyed.
		 *
		 * Call this when a Player object really needs to be destroyed.
		 */
		void reallyScheduleForDestruction();
		bool isMover() { return true; }
		bool changeArea();


		const Action* defaultAction() const { return defaultAction_; }
		void setDefaultAction(const Action& action) { defaultAction_ = &action; }
		void resetDefaultAction() { defaultAction_ = 0; }

		const Action* defaultMovementAction() const { return defaultMovementAction_; }
		void setDefaultMovementAction(const Action& action) const { defaultMovementAction_ = &action; }
		const Action* defaultTurnAction() const { return defaultTurnAction_; }
		void setDefaultTurnAction(const Action& action) const { defaultTurnAction_ = &action; }

		/**
		 *
		 */
		void planDefaultAction() const;

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

		bool findDefaultActionTarget();
		void findPickTarget();
		void resetIfTarget(Thing* target);

		Thing* pickTarget() const { return pickTarget_; }
		void setPickTarget(Thing* target) { pickTarget_ = target; }

		bool isPlayer() { return true; }

	private:
		ReportingMultiSimObject** reportingMultiSimObjects_;
		Coor lastEntrance_;


		// TODO: Move to Player class
		mutable const Action* defaultAction_;
		mutable const Action* defaultMovementAction_;
		mutable const Action* defaultTurnAction_;

		Thing* pickTarget_;
		mutable Thing* defaultActionTarget_;

	};

}

#endif
