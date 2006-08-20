#ifndef logic_area_WangAreaGrid_hpp
#define logic_area_WangAreaGrid_hpp

#include "sim/area/Area.hpp"
#include "LogicPre.hpp"

namespace se_basic {

	class _SeBasicExport WangAreaGrid {
	public:
		WangAreaGrid(int xSize, int zSize);
		~WangAreaGrid();

		void setTile(char* name, unsigned char* sides);

		inline se_core::Area* getArea(int x, int z) {
			return areas_[ x + z * xSize_ ].area_;
		}

		void addDefinition(const se_core::String& a, const unsigned char* wang) {
			WangDefinition& w = wangDefinitions_[ definitionCount_++ ];
			w.areaName_.set(a.copyValue());
			for(int i = 0; i < 4; ++i)
				w.wang_[i] = wang[i];
		}

		void setArea(int x, int z, int defIndex);
		int findMatch(const unsigned char* wang, int random);

		int getDefinition(int x, int z) {
			if(x < 0 || x >= xSize_) return -1;
			if(z < 0 || z >= zSize_) return -1;
			if(!areas_[ x + z * xSize_ ].area_) return -1;
			return areas_[ x + z * xSize_ ].definitionIndex_;
		}

		const unsigned char getSide(int x, int z, int side) {
			int def = getDefinition(x, z);
			if(def < 0) return 0;
			Assert(side >= 0 && side < 4 && "Side must be between 0 and 3");
			return wangDefinitions_[def].wang_[side];
		}

		void generate();

	private:
		struct WangDefinition {
			se_core::String areaName_;
			unsigned char wang_[4];
		} * wangDefinitions_;
		int definitionCount_;

		static const int MAX_DEFINITIONS = 128;
		struct Areas {
			se_core::Area* area_;
			int definitionIndex_;
		}* areas_;
		short xSize_, zSize_;
	};

}

#endif
