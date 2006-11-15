#include "SimplePlayerFactory.hpp"


using namespace se_core;
using namespace se_client;

namespace se_basic {

	SimplePlayerFactory
	::SimplePlayerFactory(String* name)
		: SimpleActorFactory(name) {
	}


	SimplePlayerFactory
	::~SimplePlayerFactory() {
	}


}
