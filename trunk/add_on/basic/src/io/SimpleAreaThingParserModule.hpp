#ifndef SimpleAreaThingParserModule_hpp
#define SimpleAreaThingParserModule_hpp

#include "BasicPre.hpp"

namespace se_basic {
	class _SeBasicExport SimpleAreaThingParserModule  : public se_core::ParserModule {
	public:
		SimpleAreaThingParserModule(se_core::Parser& parser);
		virtual ~SimpleAreaThingParserModule();
		void parse(se_core::InputStream& in);

	private:
		void readThing(se_core::InputStream& in, se_core::Area& area, se_core::PosNode* parent = 0);
		void readChildren(se_core::InputStream& in, se_core::Area& area, se_core::PosNode& parent);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
		void readMultiCutscene(se_core::InputStream& in, se_core::MultiSimObject& mgo);
	};

}

#endif
