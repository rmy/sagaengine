#include "util/error/Log.hpp"

namespace test {

	class SimStat {
	public:
		SimStat() {}

		void run() {
			bool success = testViewPoint();
			Assert(success && "sim_stat::ViewPoint test failed");
		}

		bool testViewPoint();
	
	};

}
