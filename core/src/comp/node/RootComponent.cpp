#include "RootComponent.hpp"
#include "comp/Composite.hpp"
#include "comp/schema/CompSchema.hpp"

namespace se_core {
	RootComponent
	::RootComponent(int type)
		: NodeComponent(type, &activeRoot()) {
	}


	Composite& RootComponent
	::activeRoot() {
		return CompSchema::activeRoot();
	}


	Composite& RootComponent
	::inactiveRoot() {
		return CompSchema::inactiveRoot();
	}
}
