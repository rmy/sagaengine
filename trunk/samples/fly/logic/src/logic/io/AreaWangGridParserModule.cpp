#include <se_core.hpp>
#include "AreaWangGridParserModule.hpp"
#include "../area/AreaWangGrid.hpp"
#include "../schema/LogicSchema.hpp"
#include <cstring>
#include <cstdio>

using namespace se_core;

namespace logic {

	AreaWangGridParserModule
	::AreaWangGridParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::GAME, ParserModule::AREA, 2)  {
	}


	void AreaWangGridParserModule
	::parse(InputStream& in) {
		Assert(!LogicSchema::areaGrid && "There is already an area grid.");

		// (width height)
		unsigned short xSize = in.readShort();
		unsigned short zSize = in.readShort();
		LogMsg(xSize << ", " << zSize);

		// Create area
		LogicSchema::areaGrid = new AreaWangGrid(xSize, zSize);

		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'T': // Tile definition
				{
					String name;
					String wang;
					in.readString(name);
					in.readString(wang);
					LogicSchema::areaGrid->addDefinition(name, (const unsigned char*)wang.get());
				}
				break;
			}
		}

		LogicSchema::areaGrid->generate();
		/*
		// (navmesh data)
		char buffer[32];
		for(int z = 0; z < zSize; ++z) {
			for(int x = 0; x < xSize; ++x) {
				sprintf(buffer, "%d.%d", x, z);
				Area* a = SimSchema::areaManager.createArea(buffer, tmp.get(), x, 0, z);
				LogicSchema::areaGrid->setArea(x, z, a);
			}
		}
		*/
	}

}
