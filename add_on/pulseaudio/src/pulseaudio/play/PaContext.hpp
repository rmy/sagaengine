#ifndef PaContext_hpp
#define PaContext_hpp

#include <pulse/pulseaudio.h>

namespace se_pulseaudio {

	class PaContext {
	public:
		PaContext(const char* name, pa_mainloop* paMainLoop);
		virtual ~PaContext();
		const char* name() { return name_; }
		pa_context* context() { return context_; }
		void setStreamListener(class PaStreamListener* listener) { listener_ = listener; }
		PaStreamListener* streamListener() { return listener_; }
		class PaStream* play(class VorbisSound& sound, int volume, bool shouldLoop = false);

		bool isReady() {
			return isReady_;
		}

		int streamsPlaying() {
			return streamCount_;
		}

		int streamCount_;
  

	private:
		static void context_state_callback(pa_context *context, void *userdata);

		const char* name_;
		pa_context *context_;
		class PaStreamListener* listener_;
		bool isReady_;
	};

}


#endif

