#include "util/error/Log.hpp"

namespace test {

	class UtilVecmath {
	public:
		UtilVecmath() {}

		void run() {
			bool success = testEuler3();
			Assert(success && "util_vecmath::Euler3 test failed");
		}

		bool testEuler3();
	
	};

}
