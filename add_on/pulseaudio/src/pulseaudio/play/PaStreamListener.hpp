#ifndef PaStreamListener_hpp
#define PaStreamListener_hpp

namespace se_pulseaudio {
	class PaStreamListener {
	public:
		virtual void streamEnded(class PaStream* stream) = 0;
		virtual void streamStopped(class PaStream* stream) = 0;
	};

}


#endif
