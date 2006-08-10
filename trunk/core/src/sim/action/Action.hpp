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


#ifndef Action_hpp
#define Action_hpp

#include "sim_action.hpp"
#include "../SimObject.hpp"
#include "../thing/sim_thing.hpp"

namespace se_core {

	/**
	 * Base class for actions.
	 * Actions are the atoms of game-character behaviour in SagaEngine.
	 * They define what a character can do.
	 */
	class _SeCoreExport Action : public SimObject {
	protected:
		/** 
		 * Constructor.
		 * @param name unique identifier for each action
		 */
		Action(const char* name);

	public:
		/**
		 * Number of timesteps that should pass before the action is performed.
		 * Returns 1 unless overridden.
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual short duration(Actor& performer, Parameter& parameter) const {
			return 1;
		}

		/**
		 * Should the action auto-repeat unless it has another action planned
		 * for this action channel?
		 * Returns false unless overridden.
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual bool isRepeating(long when, Actor &performer, Parameter& parameter) const { 
			return false;
		}

		/**
		 * Should perform be called again before the action is complete?
		 * If true is returned, the action will continue even if other
		 * actions are planned for this channel in the ActionQueue.
		 * Returns false unless overridden.
		 *
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual bool isContinuing(Actor &performer, Parameter& parameter) const {
			return false; 
		}

		/**
		 * Called when the action is inserted into the ActionQueue.
		 *
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual void prepare(Actor &performer, Parameter& parameter) const {}

		/**
		 * Called if the action is disrupted, that is being removed
		 * from the ActionQueue without being performed.
		 *
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual void disrupt(Actor &performer, Parameter& parameter) const {}

		/**
		 * Perform the action.
		 * Abstract method that must be overridden by subclasses.
		 *
		 * @param performer the param that performs this action
		 * @param parameter parameters specifying how this action should be performed
		 */
		virtual void perform(long when, Actor &performer, Parameter& parameter) const = 0;
	};

}

#endif
