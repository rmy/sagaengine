#include "BasicPre.hpp"
#include "NavMeshAreaParser.hpp"
#include "../area/NavMeshArea.hpp"
#include "../area/NavMeshAreaFactory.hpp"
#include "sim/area/AreaManager.hpp"
#include "util/type/ByteArray.hpp"

using namespace se_core;

namespace se_basic {

	NavMeshAreaParser
	::NavMeshAreaParser(ParseManager& parser)
		: Parser(parser, Parser::BASIC, Parser::AREA, 2) {
	}


	void NavMeshAreaParser
	::parse(InputStream& in) {
		// (name)
		String* name = new String();
		in.readString(*name);

		// (width height)
		unsigned short w = in.readShort();
		unsigned short h = in.readShort();

		// (navmesh data)
		unsigned short dataSize = in.readShort();
		ByteArray* data = new ByteArray();
		in.readByteArray(*data, dataSize);

		// Create area factory
		NavMeshAreaFactory* f = new NavMeshAreaFactory(name, w, h, data);
		SimSchema::areaManager.addFactory(f);

		// Create area
		// Same name, but let factory handle destruction
		//String* areaName = new String(name->get());
		//SimSchema::areaManager.createArea(areaName, name->get());

		//LogDetail(name->get() << ": " << w << ", " << h);
	}

}
