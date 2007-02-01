#include "LogicPre.hpp"
#include "ScCamera.hpp"
#include "../action/all.hpp"


using namespace se_basic;
using namespace se_core;


namespace logic {
	struct ScCamera::Data : public se_core::ReentrantData {
		Data() {}
	};


	ScCamera
	::ScCamera() : ReentrantScript("Camera") {
	}


	ScriptData* ScCamera
	::init(const se_core::ScriptComponent& performer) const {
		Data* data = new Data();
		return data;
	}


	const se_core::Action* ScCamera
	::sequence(const se_core::ScriptComponent& performer, ReentrantData& rd, Parameter& out) const {
		REENTRANT(data) {
			START;

			PERFORM(actionPlayMusic, 1);

			PERFORM(actionPopScript, 2);
		}
		LogFatal("Shouldn't happen");
		return 0;
	}


	const se_core::Action* ScCamera
	::transition(const se_core::ScriptComponent& performer, ReentrantData& rd, Parameter& out) const {
		return 0;
	}


	const ScCamera scriptCamera;
}
