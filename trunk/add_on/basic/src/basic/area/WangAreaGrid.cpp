#include "WangAreaGrid.hpp"
#include <cstdio>


using namespace se_core;

namespace se_basic {

	WangAreaGrid
	::WangAreaGrid(int xSize, int zSize) 
			: xSize_(xSize), zSize_(zSize), definitionCount_(0) {
		
		areas_ = new Areas[ xSize_ * zSize_ ];
		for(int i = 0; i < xSize_ * zSize_; ++i) {
			areas_[ i ].area_ = 0;
		}
		wangDefinitions_ = new WangDefinition[MAX_DEFINITIONS];
	}


	WangAreaGrid
	::~WangAreaGrid() {
		delete[] areas_;
		delete[] wangDefinitions_;
	}


	void WangAreaGrid
	::setArea(int x, int z, int defIndex) {
		if(defIndex < 0)
			return;

		char buffer[32];
		
		//LogMsg(defIndex << ": " << wangDefinitions_[defIndex].areaName_.get());
		// TODO: !!!!
		sprintf(buffer, "%d.%d", z, x);
		Area* a = SimSchema::areaManager.createArea(buffer, wangDefinitions_[defIndex].areaName_.get(), x, 0, z);

		areas_[ x + z * xSize_ ].area_ = a;
		areas_[ x + z * xSize_ ].definitionIndex_ = defIndex;
	}

	int WangAreaGrid
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

		char buffer[5];
		buffer[0] = (wanted[0] != 0) ? wanted[0] : 'X';
		buffer[1] = (wanted[1] != 0) ? wanted[1] : 'X';
		buffer[2] = (wanted[2] != 0) ? wanted[2] : 'X';
		buffer[3] = (wanted[3] != 0) ? wanted[3] : 'X';
		buffer[4] = 0;

		LogFatal("Didn't find match for: " << buffer);
		return 0;
	}


	void WangAreaGrid
	::generate() {
		// (navmesh data)
		unsigned char wantedWang[4];
		for(int z = 0; z < zSize_; ++z) {
			for(int x = 0; x < xSize_; ++x) {
				// Already defined?
				int myDef = getDefinition(x, z);
				if(myDef >= -1)
					continue;

				//
				wantedWang[0] = getSide(x, z - 1, 2);
				wantedWang[1] = getSide(x + 1, z, 2);
				wantedWang[2] = getSide(x, z + 1, 2);
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
