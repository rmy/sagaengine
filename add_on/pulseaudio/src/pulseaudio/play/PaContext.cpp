#include "VorbisSound.hpp"
#include "PaStream.hpp"
#include "PaContext.hpp"

#include "util/error/Log.hpp"


namespace se_pulseaudio {
	PaContext::PaContext(const char* name, pa_mainloop* paMainLoop) : name_(name), listener_(0), isReady_(false), streamCount_(0) {
		context_ = pa_context_new(pa_mainloop_get_api(paMainLoop), name_);
		if(!context_) {
			throw "Cannot create context.";
		}
		pa_context_set_state_callback(context_, PaContext::context_state_callback, this);
		pa_context_connect(context_, NULL, (pa_context_flags_t)0, NULL);
	}


	PaContext::~PaContext() {
		if(context_) {
			pa_context_disconnect(context_);
			pa_context_unref(context_);
		}
		AssertWarning(streamCount_ == 0, "There are still " << streamCount_ << " active streams.");
	}



	PaStream* PaContext::play(class VorbisSound& sound, int volume, bool shouldLoop) {
		LogDetail("Playing sound in " << name_);
		PaStream* s = new PaStream(*this, sound, volume, shouldLoop);
		return s;
	}


	void PaContext::context_state_callback(pa_context *context, void *userdata) {
		PaContext* c = (PaContext*)userdata;
		Assert(context == c->context_);

		switch (pa_context_get_state(context)) {
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			break;

		case PA_CONTEXT_READY: 
			{
				LogDetail("Context ready");
				c->isReady_ = true;
			}
			break;
		}
	}

}
