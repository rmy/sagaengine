#ifndef OgreThingParserModule_hpp
#define OgreThingParserModule_hpp

#include "io/parse/Parser.hpp"
#include "io/parse/ParserModule.hpp"
#include "io/stream/InputStream.hpp"

namespace se_asset {
	class AeO3dThingParserModule : public se_core::ParserModule {
	public:
		AeO3dThingParserModule(se_core::Parser& parser);
		void parse(se_core::InputStream& in);
	};
}


#endif
