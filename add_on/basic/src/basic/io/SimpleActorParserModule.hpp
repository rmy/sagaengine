#ifndef SimpleActorParserModule_hpp
#define SimpleActorParserModule_hpp

#include "BasicPre.hpp"
#include "../thing/SimpleActorFactory.hpp"

namespace se_basic {
	class _SeBasicExport SimpleComponentParserModule : public se_core::ComponentParserModule {
	public:
		SimpleComponentParserModule(const char* name, int type);

	private:
		se_core::DictionaryEntry name_;
	};


	class _SeBasicExport StatComponentParserModule : public SimpleComponentParserModule {
	public:
		StatComponentParserModule()
			: SimpleComponentParserModule("STAT", se_core::sct_STAT) {
		}
		se_core::SimComponentFactory* parse(se_core::InputStream& in);
	};


	class _SeBasicExport SimpleActorParserModule : public se_core::ParserModule {
	public:
		SimpleActorParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
		void readMultiCutscene(se_core::InputStream& in, se_core::MultiSimObject& mgo);
		void readMultiQuest(se_core::InputStream& in, se_core::MultiSimObject& mgo);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
		void parseStats(se_core::InputStream& in, SimpleActorFactory* factory);
		void parseSignal(se_core::InputStream& in, SimpleActorFactory* factory);
		void parsePos(se_core::InputStream& in, SimpleActorFactory* factory);

	private:
		StatComponentParserModule stat_;
	};

};


#endif
