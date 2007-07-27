#ifndef SimpleAreaThingEncoder_hpp
#define SimpleAreaThingEncoder_hpp

#include "BasicPre.hpp"
#include "io/encode/Encoder.hpp"

namespace se_basic {
	class _SeBasicExport SimpleAreaThingEncoder  : public se_core::Encoder {
	public:
		SimpleAreaThingEncoder(se_core::EncodeManager& EncodeManager);
		virtual ~SimpleAreaThingEncoder();
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
