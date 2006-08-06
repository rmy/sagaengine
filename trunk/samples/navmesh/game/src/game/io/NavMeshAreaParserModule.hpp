#ifndef game_area_NavMeshAreaParserModule_hpp
#define game_area_NavMeshAreaParserModule_hpp

#include <se_core.hpp>

namespace game {

	class NavMeshAreaParserModule : public se_core::ParserModule {
	public:
		NavMeshAreaParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
	};

}

#endif
