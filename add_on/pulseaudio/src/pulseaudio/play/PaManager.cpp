#include "PaManager.hpp"
#include "PaContext.hpp"
#include "PaStream.hpp"
#include "VorbisSound.hpp"

#include "util/error/Log.hpp"


namespace se_pulseaudio {
  PaManager::PaManager() {
    mainLoop_ = pa_mainloop_new();
    if(!mainLoop_) {
      LogFatal("No cannot create main loop");
    }


    ambience_ = new PaContext("ambience", mainLoop_);
    fx_ = new PaContext("fx", mainLoop_);
    // Wait for streams to become ready
    for(int i = 0; !(fx_->isReady() && ambience_->isReady() && i < 1024); ++i) {
      tick();
    }
  }


  PaManager::~PaManager() {
    delete fx_;
    delete ambience_;
    pa_mainloop_free(mainLoop_);
  }


  void PaManager::tick() {
    pa_mainloop_iterate(mainLoop_, 0, NULL);
  }


  class PaContext* PaManager::fx() {
    return fx_;
  }


  bool PaManager::isPlaying() {
    return fx()->streamCount_ > 0;
  }


  class PaContext* PaManager::ambience() {
    return ambience_;
  }

}
