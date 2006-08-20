#ifndef basic_area_NavMeshAreaParserModule_hpp
#define basic_area_NavMeshAreaParserModule_hpp

#include "BasicPre.hpp"

namespace se_basic {

	class _SeBasicExport NavMeshAreaParserModule : public se_core::ParserModule {
	public:
		NavMeshAreaParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
