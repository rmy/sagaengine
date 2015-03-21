#include "PaManager.hpp"
#include "PaContext.hpp"
#include "PaStream.hpp"
#include "VorbisSound.hpp"

#include "util/error/Log.hpp"


namespace se_pulseaudio {
	PaManager::PaManager() : isOk_(false) {
		mainLoop_ = pa_mainloop_new();
		if(!mainLoop_) {
			LogWarning("No cannot create main loop");
		}

		ambience_ = new PaContext("ambience", mainLoop_);
		fx_ = new PaContext("fx", mainLoop_);
		// Wait for streams to become ready
		//pa_mainloop_iterate(mainLoop_, 1, NULL);
		for(int i = 0; !(fx_->isReady() && ambience_->isReady() && i < 1024); ++i) {
			tick();
		}
		isOk_ = (ambience_ != 0 && fx_ != 0);
	}


	PaManager::~PaManager() {
		if(fx_) delete fx_;
		if(ambience_) delete ambience_;
		if(mainLoop_) pa_mainloop_free(mainLoop_);
		LogDetail("Freed pulseaudio main loop");
	}


	void PaManager::tick() {
		pa_mainloop_iterate(mainLoop_, 0, NULL);
	}


	class PaContext* PaManager::fx() {
		return fx_;
	}


	bool PaManager::isPlaying() {
		return fx()->streamsPlaying() > 0;
	}


	class PaContext* PaManager::ambience() {
		return ambience_;
	}

}
