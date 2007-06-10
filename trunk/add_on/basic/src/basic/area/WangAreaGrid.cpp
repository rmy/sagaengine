#include "WangAreaGrid.hpp"
#include "sim/stat/DictionaryEntry.hpp"
#include "sim/area/AreaManager.hpp"
#include <cstdio>


using namespace se_core;

namespace se_basic {

	WangAreaGrid
	::WangAreaGrid(int xSize, int zSize, const char* name) 
			: xSize_(xSize), zSize_(zSize), definitionCount_(0), defaultBorder_('0') {

		areas_ = new Areas[ xSize_ * zSize_ ];
		for(int i = 0; i < xSize_ * zSize_; ++i) {
			areas_[ i ].area_ = 0;
			areas_[ i ].definitionIndex_ = -2;
		}
		wangDefinitions_ = new WangDefinition[MAX_DEFINITIONS];
		dict_ = new DictionaryEntry(DE_ZONE, name, true);
	}


	WangAreaGrid
	::~WangAreaGrid() {
		delete[] areas_;
		delete[] wangDefinitions_;
	}


	void WangAreaGrid
	::setArea(int x, int z, int defIndex) {

		areas_[ x + z * xSize_ ].definitionIndex_ = defIndex;
		if(defIndex < 0)
			return;
		char buffer[32];		
		sprintf(buffer, "%s/%d.%d", dict_->name_, z, x);
		Area* a = SimSchema::areaManager.createArea(buffer, wangDefinitions_[defIndex].areaName_.get(), x, 0, z, dict_->id_);
		LogDetail("Grid: " << dict_->name_ << " - " << dict_->id_);

		areas_[ x + z * xSize_ ].area_ = a;
	}


	int WangAreaGrid
	::findMatch(const unsigned char* wanted, int random) {
		//LogDetail("Defs: " << definitionCount_);
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

		for(int i = 0; i < 4; ++i) {
			if(wanted[i] == '0' || wanted[i] == 0) {
				return -1;
			}
		}

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
	::generate(int seed) {
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
				wantedWang[1] = getSide(x + 1, z, 3);
				wantedWang[2] = getSide(x, z + 1, 0);
				wantedWang[3] = getSide(x - 1, z, 1);

				seed += x * z;
				seed ^= (seed >> 8);
				seed ^= (seed >> 16);
				seed ^= (seed >> 24);
				int def = findMatch(wantedWang, seed % 19);

				setArea(x, z, def);
			}
		}
	}


	void WangAreaGrid
	::save(const char* filename) {
		char fullPath[256];
		sprintf(fullPath, "%s/logic/area/grid/%s.txt", IoSchema::dataPath, filename);
		FILE* out = fopen(fullPath, "wt");

		fprintf(out, "XG02\n");
		fprintf(out, "%d %d\n", xSize_, zSize_);
		for(int i = 0; i < definitionCount_; ++i) {
			WangDefinition* w = &wangDefinitions_[i];
			fprintf(out, "# %d\n", i);
			fprintf(out, "T %s %c%c%c%c\n", w->areaName_.get(), w->wang_[0], w->wang_[1], w->wang_[2], w->wang_[3]);
		}
		fprintf(out, "\n");

		for(int z = 0; z < zSize_; ++z) {
			fprintf(out, "C %d { ", z);
			for(int x = 0; x < xSize_; ++x) {
				int myDef = getDefinition(x, z);
				fprintf(out, "%2d ", myDef);
			}
			fprintf(out, "}\n");
		}
		fprintf(out, "Q\n");
		fclose(out);
	}

}
