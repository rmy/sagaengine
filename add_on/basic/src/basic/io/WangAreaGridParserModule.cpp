#include <se_core.hpp>
#include "WangAreaGridParserModule.hpp"
#include "../area/WangAreaGrid.hpp"
#include <cstring>
#include <cstdio>

using namespace se_core;

namespace se_basic {

	WangAreaGridParserModule
	::WangAreaGridParserModule(Parser& parser)
		: ParserModule(parser, ParserModule::GAME, ParserModule::AREA_GRID, 2)  {
	}


	void WangAreaGridParserModule
	::parse(InputStream& in) {
		// (width height)
		unsigned short xSize = in.readShort();
		unsigned short zSize = in.readShort();

		// Create area
		WangAreaGrid grid(xSize, zSize);

		int code;
		while((code = in.readInfoCode()) != 'Q') {
			switch(code) {
			case 'T': // Tile definition
				{
					String name;
					String wang;
					in.readString(name);
					in.readString(wang);
					grid.addDefinition(name, (const unsigned char*)wang.get());
				}
				break;

			case 'R':
				{
					short z = in.readShort();
					Assert(z < zSize);
					int c = in.readInfoCode();
					Assert(c == '{');
					int x = 0;
					while(x < xSize) {
						int def = in.readShort();
						grid.setArea(z, x, def);
						++x;
					}
					c = in.readInfoCode();
					Assert(c == '}');
				}
			}
		}

		grid.generate();
	}

}
