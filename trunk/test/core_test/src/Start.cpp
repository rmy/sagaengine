#include "UtilVecmath.hpp"
#include "SimStat.hpp"
#include "util/error/Log.hpp"
#include <cstdio>


namespace se_err {
	void scream3(const char* file, int line, const char* msg) {
		// Fatal
		char* buffer = new char[512];
		sprintf(buffer, "Fat: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);

		throw buffer;
	}


	void say3(const char* file, int line, const char* msg) {
		// Msg
		char buffer[512];
		sprintf(buffer, "Msg: \"%s\" (%s-%d)", msg, file, line);
		dump(buffer);
	}


	void dump(const char *msg) {
		puts(msg);
	}

} // namespace se_err


int main() {
	LogMsg("------------- Entered main ----------------");
	{ // Test util_vecmath
		LogMsg("Testing util_vecmath");
		test::UtilVecmath test;
		test.run();
	}

	{ // Test sim_stat
		LogMsg("Testing sim_stat");
		test::SimStat test;
		test.run();
	}

	LogMsg("------------- Leaving main ----------------");
}
