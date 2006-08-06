#include "SimStat.hpp"
#include "sim/stat/ViewPoint.hpp"
#include "util/type/util_type.hpp"

namespace test {
	using namespace se_core;

	bool SimStat
	::testViewPoint() {
		ViewPoint vp1;
		ViewPoint vp2;

		bool success = true;
		for(int i = -360; i < 360; i += 10) {
			vp1.localCoor().set(i, i, i);
			vp1.localFace().setEuler(BRAY_RES * i, BRAY_RES * i, BRAY_RES * i);
			vp1.localFace().normalize();

			vp2 = vp1;
			vp2.add(vp1);
			vp2.sub(vp1);
			if(!vp2.viewPointEquals(vp1, BRAY_RES, COOR_RES / 4)) {
				LogMsg("ViewPoint add/sub test failed");
				LogMsg(vp1.toLog());
				LogMsg(vp2.toLog());
				success = false;
			}
		}

		return success;
	}

}
