#ifndef basic_area_NavMeshAreaParser_hpp
#define basic_area_NavMeshAreaParser_hpp

#include "BasicPre.hpp"
#include "io/parse/Parser.hpp"

namespace se_basic {

	class _SeBasicExport NavMeshAreaParser : public se_core::Parser {
	public:
		NavMeshAreaParser(se_core::ParseManager& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
