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


#ifndef sim_stat_Anim_hpp
#define sim_stat_Anim_hpp

#include "util/type/util_type.hpp"
#include "../config/sim_config.hpp"

namespace se_core {
	class _SeCoreExport Anim {
	public:
		typedef short MovementMode;

		Anim();
		void setAnim(const Anim& original);

		/**
		 * Set the movement mode.
		 * The movement mode is used by the graphics engine to choose wich
		 * animation to choose for an in game thing.
		 * This simplified version has no when parameter, and should only
		 * be used for animations that does not should continue using the
		 * started when of the previous movement mode, or of the time
		 * when the movement started has no significance.
		 *
		 * @param mode The new movement mode.
		 */
		void setMovementMode(short mode);

		/**
		 * Set the movement mode.
		 * The movement mode is used by the graphics engine to choose wich
		 * animation to choose for an in game thing.
		 * This is the full version, setting both the time when the movement
		 * started and the mode itself.
		 *
		 * @param when The adjusted game clock for when the movement started (milliseconds).
		 * @param mode The new movement mode.
		 */
		void setMovementMode(long when, short mode);

		/**
		 * Get the present movement mode.
		 *
		 * @return The present movement mode.
		 */
		short movementMode() const;

		/**
		 * Get long this movement mode has been active.
		 * Used to check how far into an animation we are Use modulus (%) to
		 * for repeating animations.
		 *
		 * @param when The present adjusted game clock (milliseconds).
		 * @return The present gameClock - when the movement mode started
		 */
		long movementWhen(long when) const { return when - states_[0].movementStartedWhen_; }
		long movementWhen(int index, long when) const { return when - states_[0].movementStartedWhen_; }

		static void setMovementModeCount(int c) { MOVEMENT_MODE_COUNT = c; }
		static int MOVEMENT_MODE_COUNT;

	private:
		enum { MAX_ANIMS = 4 };

		struct _SeCoreExport State {
			State() :
				movementMode_(0),
				movementStartedWhen_(0),
				weight_(0),
				isFrozen_(false),
				isActive_(false) {				
			}

			/** The movement mode. Signifies which animation should be shown. */
			short movementMode_;

			/** The time when the movement mode (animation) started. */
			long movementStartedWhen_;

			scale_t weight_;

			bool isFrozen_;

			bool isActive_;
		} states_[ MAX_ANIMS ];

		
	};

}

#endif
