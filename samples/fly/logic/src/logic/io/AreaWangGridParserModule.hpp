#ifndef logic_area_AreaWangGridParserModule_hpp
#define logic_area_AreaWangGridParserModule_hpp

#include <se_core.hpp>

namespace logic {

	class _FlyLogicExport AreaWangGridParserModule : public se_core::ParserModule {
	public:
		AreaWangGridParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
