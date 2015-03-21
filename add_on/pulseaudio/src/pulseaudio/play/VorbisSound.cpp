#include "VorbisSound.hpp"
#include "util/error/Log.hpp"

#include <string.h>
#include <vorbis/vorbisfile.h>

namespace se_pulseaudio {

	VorbisSound::VorbisSound(const char* filename) : ogg_(0), oggSize_(0), sampleRate_(44100), channels_(0) {
		FILE* infile = fopen(filename, "rb");
		LogDetail("Loading: " << filename);
		init(infile);
		fclose(infile);
	}


	VorbisSound::VorbisSound(FILE* infile) : ogg_(0), oggSize_(0), sampleRate_(44100), channels_(0) {
		oggSize_ = 0;
		init(infile);
	}


	void VorbisSound::init(FILE* infile) {
		static char buffer[256 * 1024 * 1024];
		int bufferSize = 0;
		if (infile) {
			bufferSize = fread(buffer, 1, sizeof(buffer), infile);
		}
		oggSize_ = bufferSize;
		ogg_ = new char[ bufferSize ];
		memcpy(ogg_, buffer, oggSize_);
		LogDetail("Sound size: " << bufferSize);

		OggVorbis_File* f = open();
		sampleRate_ = ov_info(f, 0)->rate;
		channels_ = ov_info(f, 0)->channels;
		close(f);
		
	}


	VorbisSound::~VorbisSound() {
		delete[] ogg_;
	}


	struct MEMFILE {
		MEMFILE(char *b, size_t size) : offset_(0), size_(size), buffer_(b) {}

		size_t offset_;
		size_t size_;
		char* buffer_;


		static size_t fread(void *ptr, size_t size, size_t nmemb, void *datasource) {
			MEMFILE* f = (MEMFILE*)datasource;
			size_t offset = f->offset_;
			size_t bytesRead = (offset + size * nmemb <= f->size_) ? size * nmemb : f->size_ - offset;
			f->offset_ += bytesRead;
			memcpy((char*)ptr, f->buffer_ + offset, bytesRead);

			return bytesRead;
		}

		static size_t fclose(void *datasource) {
			MEMFILE* f = (MEMFILE*)datasource;
			delete f;
		}
	};


	OggVorbis_File* VorbisSound::open() {
		static ov_callbacks OV_CALLBACKS = {
			(size_t (*)(void *, size_t, size_t, void *))  MEMFILE::fread,
			(int (*)(void *, ogg_int64_t, int))           NULL,
			(int (*)(void *))                             MEMFILE::fclose,
			(long (*)(void *))                            NULL
		};
		OggVorbis_File* vf = new OggVorbis_File();
		MEMFILE* infile = new MEMFILE(ogg_, oggSize_);
		if(ov_open_callbacks(infile, vf, NULL, 0, OV_CALLBACKS) < 0) {
			LogWarning("Something rotten when opening memfile for ogg parsing");
			delete vf;
			return 0;
		}

		return vf;
	}


	int VorbisSound::read(char *pcmout, int length, OggVorbis_File* vf, int &currentSection) {
		int bytesRead = ov_read(vf, pcmout, length, 0, 2, 1, &currentSection);
		return bytesRead;
	}


	void VorbisSound::close(OggVorbis_File* vf) {
		ov_clear(vf);
		if(vf) {
			delete vf;
		}
	}

}
