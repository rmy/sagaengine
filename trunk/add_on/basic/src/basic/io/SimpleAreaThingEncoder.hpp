#ifndef SimpleAreaThingEncoderModule_hpp
#define SimpleAreaThingEncoderModule_hpp

#include "BasicPre.hpp"
#include "io/encode/EncoderModule.hpp"

namespace se_basic {
	class _SeBasicExport SimpleAreaThingEncoderModule  : public se_core::EncoderModule {
	public:
		SimpleAreaThingEncoderModule(se_core::Encoder& Encoder);
		virtual ~SimpleAreaThingEncoderModule();
		void encode(se_core::OutputStream& in);

		/*
	private:
		void readThing(se_core::InputStream& in, int areaCount, se_core::Area** areas, se_core::Composite** parents = 0);
		void readChildren(se_core::InputStream& in, int areaCount, se_core::Area** areas, se_core::Composite** parents);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
		void readMultiCutscene(se_core::InputStream& in, int areaCount, se_core::Area** areas);
		*/
	};

}

#endif
