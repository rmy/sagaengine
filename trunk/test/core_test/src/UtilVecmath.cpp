#include "UtilVecmath.hpp"
#include "util/type/util_type.hpp"
#include "util/vecmath/all.hpp"

using namespace se_core;

namespace test {

	bool UtilVecmath
	::testEuler3() {
		bool success = true;

		return success;
	}


	bool UtilVecmath
	::testViewPoint() {
		ViewPoint vp1;
		ViewPoint vp2;

		bool success = true;
		for(int i = -360; i < 360; i += 10) {
			vp1.coor_.set(i, i, i);
			vp1.face_.setEuler(BRAY_RES * i, BRAY_RES * i, BRAY_RES * i);
			vp1.face_.normalize();

			vp2 = vp1;
			vp2.add(vp1);
			vp2.sub(vp1);
			if(!vp2.viewPointEquals(vp1, COOR_RES / 4, BRAY_RES)) {
				LogMsg("ViewPoint add/sub test failed");
				LogMsg(vp1.toLog());
				LogMsg(vp2.toLog());
				success = false;
			}
		}

		return success;
	}


}
