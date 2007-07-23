#ifndef logic_area_WangAreaGridParser_hpp
#define logic_area_WangAreaGridParser_hpp

#include <se_core.hpp>
#include "io/parse/Parser.hpp"

namespace se_basic {

	class _SeBasicExport WangAreaGridParser : public se_core::Parser {
	public:
		WangAreaGridParser(se_core::ParseManager& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
