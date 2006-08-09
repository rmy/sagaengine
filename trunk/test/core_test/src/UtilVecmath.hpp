#include "util/error/Log.hpp"

namespace test {

	class UtilVecmath {
	public:
		UtilVecmath() {}

		void run() {
			bool success = testEuler3();
			Assert(success && "util_vecmath::Euler3 test failed");
			success = testViewPoint();
			Assert(success && "sim_stat::ViewPoint test failed");
		}

		bool testEuler3();
		bool testViewPoint();
	
	
	};

}
