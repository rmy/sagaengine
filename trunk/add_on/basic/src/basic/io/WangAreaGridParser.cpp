#include <se_core.hpp>
#include "WangAreaGridParser.hpp"
#include "../area/WangAreaGrid.hpp"
#include <cstring>
#include <cstdio>

using namespace se_core;

namespace se_basic {

	WangAreaGridParser
	::WangAreaGridParser(ParseManager& parser)
		: Parser(parser, Parser::GAME, Parser::AREA_GRID, 2)  {
	}


	void WangAreaGridParser
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
#ifdef SE_SAVE_GRID
					grid.save(tmp.get());
#endif
					break;
				}
			}

		}

	}

}
