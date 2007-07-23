#ifndef SimpleAreaThingParser_hpp
#define SimpleAreaThingParser_hpp

#include "BasicPre.hpp"
#include "io/parse/Parser.hpp"

namespace se_basic {
	class _SeBasicExport SimpleAreaThingParser  : public se_core::Parser {
	public:
		SimpleAreaThingParser(se_core::ParseManager& parser);
		virtual ~SimpleAreaThingParser();
		void parse(se_core::InputStream& in);

	private:
		void readThing(se_core::InputStream& in, int areaCount, se_core::Area** areas, se_core::Composite** parents = 0);
		void readChildren(se_core::InputStream& in, int areaCount, se_core::Area** areas, se_core::Composite** parents);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
		void readMultiCutscene(se_core::InputStream& in, int areaCount, se_core::Area** areas);
	};

}

#endif
