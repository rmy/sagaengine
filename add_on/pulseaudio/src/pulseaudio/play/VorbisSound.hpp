#ifndef VorbisSound_hpp
#define VorbisSound_hpp

#include <vorbis/vorbisfile.h>

namespace se_pulseaudio {
	class VorbisSound {
	public:
		VorbisSound(const char* filename);
		VorbisSound(FILE* infile);
		void init(FILE* infile);
  
		~VorbisSound();

		OggVorbis_File* open();
		int read(char *pcmout, int length, OggVorbis_File* vf, int &current_section);
		int close(OggVorbis_File* vf, int &current_section);

	private:
		int oggSize_;
		char* ogg_;
	};
}

#endif
