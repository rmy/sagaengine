#ifndef game_action_ShootAcid_hpp
#define game_action_ShootAcid_hpp

#include "sim/action/Action.hpp"


namespace game {
	class ShootAcid : public se_core::Action {
	public:
		ShootAcid() : se_core::Action("laser_attack") {}

		short duration(se_core::Actor& performer, se_core::Parameter& parameter) const;
		bool isContinuing(se_core::Actor &performer, se_core::Parameter& parameter) const;

		short channel() const { return se_core::CHANNEL_EXTRA; }

		bool isRepeating(long when, se_core::Actor &performer, se_core::Parameter& parameter) const {
			return false;
		}

		void perform(long when, se_core::Actor& performer, se_core::Parameter& parameter) const;

	private:
		void spawn(long when, se_core::Actor& performer) const;
	};

	extern const ShootAcid actionShootAcid;
}

#endif
