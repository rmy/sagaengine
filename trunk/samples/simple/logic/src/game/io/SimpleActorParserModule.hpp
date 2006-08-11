#ifndef SimpleActorParserModule_hpp
#define SimpleActorParserModule_hpp

#include "GamePre.H"

namespace game {

	class SimpleActorParserModule : public se_core::ParserModule {
	public:
		SimpleActorParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
		void readMultiCutscene(se_core::InputStream& in, se_core::MultiSimObject& mgo);
		void readMultiQuest(se_core::InputStream& in, se_core::MultiSimObject& mgo);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
	};

};


#endif
