#ifndef PaManager_hpp
#define PaManager_hpp

#include <pulse/mainloop.h>

namespace se_pulseaudio {
  class PaManager {
  public:
    PaManager();
    ~PaManager();
    void tick();
    class PaContext* fx();
    class PaContext* ambience();
    bool isPlaying();

  private:
    pa_mainloop *mainLoop_;

    class PaContext *ambience_;
    static const int FX_COUNT = 7;
    class PaContext *fx_;
  };
}

#endif
