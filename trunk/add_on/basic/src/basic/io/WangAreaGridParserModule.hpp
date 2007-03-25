#ifndef logic_area_WangAreaGridParserModule_hpp
#define logic_area_WangAreaGridParserModule_hpp

#include <se_core.hpp>
#include "io/parse/ParserModule.hpp"

namespace se_basic {

	class _SeBasicExport WangAreaGridParserModule : public se_core::ParserModule {
	public:
		WangAreaGridParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
