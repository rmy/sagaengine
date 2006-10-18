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
		long movementWhen(long when) const { return when - movementStartedWhen_; }
		void setSpeed(scale_t s) { speed_ = s; }
		scale_t speed() const { return speed_; }

		void setWeight(scale_t w) { weight_ = w; }
		scale_t weight() const { return weight_; }
		/**
		 * @returns 0 for beginning of anim, SCALE_RES for end of anim.
		 */
		void setStartPos(float p) { startPos_ = p; }
		void addStartPos(float p) { startPos_ += p; }
		scale_t startPos() const { return startPos_; }
		scale_t pos() const { return pos_; }
		scale_t resetPos() { pos_ = startPos_; }
		scale_t addPos(scale_t s) { pos_ += s; }
		scale_t updatePos() { pos_ += speed_ * SCALE_STEP; }

		static void setMovementModeCount(int c) { MOVEMENT_MODE_COUNT = c; }
		static int MOVEMENT_MODE_COUNT;

	private:
		/** The movement mode. Signifies which animation should be shown. */
		short movementMode_;

		/** The time when the movement mode (animation) started. */
		long movementStartedWhen_;

		scale_t pos_;

		scale_t weight_;

		scale_t speed_;

		scale_t startPos_;
	};

}

#endif
