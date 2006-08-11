#ifndef SimplePlayerFactory_hpp
#define SimplePlayerFactory_hpp

#include "LogicPre.hpp"
#include "SimpleActorFactory.hpp"

namespace logic {

	class SimplePlayerFactory : public SimpleActorFactory {
	public:
		SimplePlayerFactory(se_core::String* name);
		~SimplePlayerFactory();

		se_core::Thing* create() const;
	};

}

#endif
