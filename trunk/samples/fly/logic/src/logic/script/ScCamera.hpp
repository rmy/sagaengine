#ifndef logic_ScCamera_hpp
#define logic_ScCamera_hpp

#include "sim/script/ReentrantScript.hpp"


namespace logic {
	extern _FlyLogicExport const class _FlyLogicExport ScCamera : public se_core::ReentrantScript {
	public:
		ScCamera();

		se_core::ScriptData* init(const se_core::Actor& performer) const;
		const se_core::Action* sequence(const se_core::Actor& performer, se_core::ReentrantData& _, se_core::Parameter& out) const;
		const se_core::Action* transition(const se_core::Actor& performer, se_core::ReentrantData& _, se_core::Parameter& out) const;
		bool isStacker() const { return false; }

	private:
		struct Data;
	} scriptCamera;
}

#endif
