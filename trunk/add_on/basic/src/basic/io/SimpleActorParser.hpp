#ifndef SimpleActorParser_hpp
#define SimpleActorParser_hpp

#include "BasicPre.hpp"
#include "../thing/SimpleActorFactory.hpp"
#include "io/parse/component/ComponentParser.hpp"
#include "io/parse/Parser.hpp"
#include "comp/list/comp_list.hpp"

namespace se_basic {
	class _SeBasicExport SimpleComponentParser : public se_core::ComponentParser {
	public:
		SimpleComponentParser(const char* name, int type);

	private:
		se_core::DictionaryEntry name_;
	};


	class _SeBasicExport SignalComponentParser : public SimpleComponentParser {
	public:
		SignalComponentParser()
			: SimpleComponentParser("SIGNAL", se_core::sct_SIGNAL) {
		}
		se_core::ComponentFactory* parse(se_core::InputStream& in);
	};


	class _SeBasicExport SimpleActorParser : public se_core::Parser {
	public:
		SimpleActorParser(se_core::ParseManager& parser);
		void parse(se_core::InputStream& in);
		void readMultiCutscene(se_core::InputStream& in, se_core::ObjectList& mgo);
		void readMultiQuest(se_core::InputStream& in, se_core::ObjectList& mgo);
		void readSpawnPoint(se_core::InputStream& in, se_core::ViewPoint& sp);
		//void parseStats(se_core::InputStream& in, SimpleActorFactory* factory);
		void parsePos(se_core::InputStream& in, SimpleActorFactory* factory);

	private:
		SignalComponentParser signal_;
	};

};


#endif
