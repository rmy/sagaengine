#ifndef SimpleAreaThingParserModule_hpp
#define SimpleAreaThingParserModule_hpp

#include <se_core.hpp>

namespace se_core {
	class SimpleAreaThingParserModule  : public se_core::ParserModule {
	public:
		SimpleAreaThingParserModule(se_core::Parser& parser);
		virtual ~SimpleAreaThingParserModule();
		void parse(se_core::InputStream& in);

	private:
		void readThing(se_core::InputStream& in, se_core::Area& area, se_core::PosNode* parent = 0);
		void readChildren(se_core::InputStream& in, se_core::Area& area, se_core::PosNode& parent);
		void readSpawnPoint(InputStream& in, SpawnPoint& sp);
		void readMultiCutscene(se_core::InputStream& in, se_core::MultiSimObject& mgo);
	};

}

#endif
