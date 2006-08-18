#include "AreaWangGrid.hpp"
#include <cstdio>


using namespace se_core;

namespace logic {

	AreaWangGrid
	::AreaWangGrid(int xSize, int zSize) 
			: xSize_(xSize), zSize_(zSize), definitionCount_(0) {
		
		areas_ = new Areas[ xSize_ * zSize_ ];
		for(int i = 0; i < xSize_ * zSize_; ++i) {
			areas_[ i ].area_ = 0;
		}
		wangDefinitions_ = new WangDefinition[MAX_DEFINITIONS];
	}


	AreaWangGrid
	::~AreaWangGrid() {
		delete[] areas_;
		delete[] wangDefinitions_;
	}


	void AreaWangGrid
	::setArea(int x, int z, int defIndex) {
		char buffer[32];
		

		//LogMsg(defIndex << ": " << wangDefinitions_[defIndex].areaName_.get());
		sprintf(buffer, "%d.%d", x, z);
		Area* a = SimSchema::areaManager.createArea(buffer, wangDefinitions_[defIndex].areaName_.get(), x, 0, z);

		areas_[ x + z * xSize_ ].area_ = a;
		areas_[ x + z * xSize_ ].definitionIndex_ = defIndex;
	}

	int AreaWangGrid
	::findMatch(const unsigned char* wanted, int random) {
		//LogMsg("Defs: " << definitionCount_);
		bool didFind = false;
		do {
			for(int i = 0; i < definitionCount_; ++i) {
				WangDefinition& w = wangDefinitions_[i];
				bool isMatch = true;

				for(int j = 0; j < 4; ++j) {
					if(wanted[j] != 0 && wanted[j] != w.wang_[j]) {
						isMatch = false;
					}
				}
				if(isMatch) {
					if(random <= 0) {
						return i;
					}
					--random;
					didFind = true;
				}
			}
		} while(didFind);

		Assert("Didn't find match");
		return 0;
	}


	void AreaWangGrid
	::generate() {
		// (navmesh data)
		unsigned char wantedWang[4];
		for(int z = 0; z < zSize_; ++z) {
			for(int x = 0; x < xSize_; ++x) {
				wantedWang[0] = getSide(x, z - 1, 2);
				wantedWang[1] = 0;
				wantedWang[2] = 0;
				wantedWang[3] = getSide(x - 1, z, 1);

				int def = findMatch(wantedWang, x * z % 19);

				if(!wantedWang[0]) wantedWang[0] = '-';
				if(!wantedWang[3]) wantedWang[3] = '-';

				//LogMsg(def << ": " << (char)wantedWang[0] << "--" << (char)wantedWang[3]);

				setArea(x, z, def);
			}
		}
	}
}
