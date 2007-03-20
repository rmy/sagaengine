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


#ifndef sim_SimEngine_hpp
#define sim_SimEngine_hpp

#include "sim.hpp"
#include "./config/sim_config.hpp"
#include "./stat/sim_stat.hpp"
#include "./thing/sim_thing.hpp"
#include "./area/sim_area.hpp"
#include "util/vecmath/Vector3.hpp"
#include "util/vecmath/Point3.hpp"


namespace se_core {

	/**
	 * SimEngine is the main manager class for the simulation engine.
	 * You call the SimSchema::simEngine->step() method to step
	 * the simulation engine.
	 */
	class _SeCoreExport SimEngine {
	public:
		/**
		 * Constructor.
		 */
		SimEngine();

		/**
		 * Destructor.
		 */
		~SimEngine();

		/**
		 * Initialise the simulation engine. This should be called once
		 * after the application starts up.
		 *
		 * It will cast an InitListener::initEngineEvent(), so make sure that
		 * listeners that should receive this event is registered first.
		 */
		static bool init();

		const char* level() { return level_.get(); }


		/**
		 * Cleanup the simulation engine. This should be called once,
		 * before application shuts down.
		 *
		 * It will cast an InitListener::cleanupEngineEvent() to
		 * registered listeners.
		 */
		void cleanup();


		/**
		 * Initialise engine for a new game.
		 *
		 * It will cast an InitListener::initGameEvent() to registered
		 * listeners.
		 */
		void initGame();


		/**
		 * Reset game. Cleans up all areas and prepares the engine
		 * for a new game.
		 *
		 * It will cast an InitListener::cleanupGameEvent() to
		 * registered listeners.
		 */
		void cleanupGame();

		void setLevel(const char* name);

		/**
		 * Performs simulation step and calls listeners.
		 * Call this method as often as possible within the game loop. Performs
		 * all AI actions. Also casts events that lower layers may hook on to
		 * to process stuff before the AI processing starts, or after the
		 * AI has caught up and become up to date.
		 *
		 * @param when The present clock (milliseconds)
		 */
		bool step(long when);


		/**
		 * Game loop.
		 *
		 * @param when The present clock (milliseconds)
		 */
		void go();


		/**
		 * Reset the engine. After this, everything has to be loaded again.
		 */
		void resetAll();


		/**
		 * Reset step counter.
		 */
		void resetTimer() { previousPerform_ = 0; }


		/**
		 * Set or reset the multiple perform() flag.
		 * If true, the step() method will perform AI logic more than
		 * once if the AI has lagged behind the game clock.
		 * This means that movement will be smooth, but that screen updates
		 * may be skipped.
		 *
		 * If false, the AI will just skip the AI performs it
		 * is lagged behind ignoring the fact that it has lagged behind.
		 * This may make game movements jerky, but will make it
		 * easier to have a consistent number of screen updates per AI
		 * perform().
		 *
		 * @param state New state
		 */
		void setMultiplePerformsPerStepEnabled(bool state);


		/**
		 * Get the adjusted game clock.
		 * The adjusted game clock for the presently perform()ing or last
		 * peform()ed AI step.
		 *
		 * @return Adjusted game clock (milliseconds)
		 */
		inline long when() {
			return (previousPerform_ - lostPerformAdjustment_) << TIMESTEP_INTERVAL_SHIFT;
		}

		/**
		 * Returns value of game over flag.
		 *
		 * @return true, game is over, false, game is not over.
		 */
		bool isGameOver() { return isGameOver_; }

		/**
		 * Set the game over flag
		 *
		 * @param state true, game is over, false, game is not over.
		 */
		void setGameOver(bool state);

	protected:
		/**
		 * Performs AI actions.
		 *
		 * Algorithm as follow:
		 *   move() - precalced next coor becomes present coor
		 *   slide() - slide on walls
		 *   for all action channels:
		 *     actionQueue.performScheduledActions() - performs actions
		 *   for all action channels:
		 *     actionQueue.scheduleNextActions() - scripts plans next action
		 *   thingFactory.performDestructions() - destroy those who died this step
		 *   apply() - precalc next coor
		 *   testCollisions() - test and adjust for collisions
		 *   for all action channels:
		 *     actionQueue.beginNextInitiative() - cleans up action queue
		 *   update scriptTracker - updates script (cutscene) tracker
		 *
		 * @param when Ajusted game clock (milliseconds) for the present step (adjusted for
		 *   pauses, frame skipping, etc).
		 */
		void perform(long when);


	private:
		/**
		 * Tick counter.
		 */
		long previousPerform_;

		/**
		 * Game over flag.
		 */
		bool isGameOver_;

		/**
		 * If true, the step() method will never perform
		 * more than one AI step. If the AI is not up to date
		 * with the game clock, then lost steps will be skipped,
		 * with the result that the game will 'jerk'.
		 */
		bool multiplePerformsPerStepEnabled_;

		/**
		 * Game clock adjustment.
		 */
		long lostPerformAdjustment_;

		String level_, nextLevel_;
	};

}

#endif
