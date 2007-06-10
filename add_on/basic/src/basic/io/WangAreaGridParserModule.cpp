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

		// Strip extension from basename, so that
		// .bin and .txt versions of the same grid produce
		// the same hash id
		String name(in.basename(), -in.extLen());

		// Create area
		WangAreaGrid grid(xSize, zSize, name.get());
		int seed = 19;

		int code;
		while((code = in.readInfoCode()) != 'Q' && !in.eof()) {
			switch(code) {
			case 'S':
				seed = in.readShort();
				break;

			case 'T': // Tile definition
				{
					String name;
					String wang;
					in.readString(name);
					in.readString(wang);
					grid.addDefinition(name, (const unsigned char*)wang.get());
				}
				break;

			case 'C':
				{
					short z = in.readShort();
					AssertFatal(z < zSize, in.name());
					int c = in.readInfoCode();
					AssertFatal(c == '{', "Missing { in '" << in.name() << "' - C: " << z);
					int x = 0;
					while(x < xSize) {
						short def = in.readShort();
						grid.setArea(x, z, def);
						++x;
					}
					c = in.readInfoCode();
					AssertFatal(c == '}', "Missing } in '" << in.name() << "' - C: " << z);
				}
				break;

			case 'B':
				{
					int c = in.readShort();
					grid.setDefaultBorder(c);
				}
				break;

			case 'G':
				grid.generate(seed);
				break;

			case 'O':
				{
					String tmp;
					in.readString(tmp);
					grid.save(tmp.get());
					break;
				}
			}

		}

	}

}
