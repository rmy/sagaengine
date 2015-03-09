#ifndef PaStream_hpp
#define PaStream_hpp

#include <pulse/pulseaudio.h>
#include "VorbisSound.hpp"

namespace se_pulseaudio {

	class PaStream {
	public:
		PaStream(class PaContext &context, class VorbisSound& sound, int volume, bool shouldLoop);
		~PaStream();
		void setVolume(int v);
		int doStreamWrite(size_t length);
		void doStreamWrite();

		void play();
		void stop();
		bool isPlaying() { return sound_ != 0 || isDraining_; }

		static void stream_write_callback(pa_stream *s, size_t length, void *userdata);
		static void stream_state_callback(pa_stream *s, void *userdata);
		static void stream_drained_callback(pa_stream *stream, int success, void *userdata);
		static void stream_corked_callback(pa_stream *stream, int success, void *userdata);

		PaContext& context() { return context_; }
		OggVorbis_File* vorbisFile() { return vorbisFile_; }
		

	private:
		class PaContext& context_;
		pa_stream *stream_;

		class VorbisSound *sound_;
		OggVorbis_File* vorbisFile_;
		bool shouldLoop_;

		bool isDraining_;
		int currentSection_;
		int bytesRead_;
		int bytesWritten_;
		char pcmout_[16 * 1024];

		char name_[128];
	};
}


#endif
