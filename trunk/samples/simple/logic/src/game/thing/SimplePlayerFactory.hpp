#ifndef SimplePlayerFactory_hpp
#define SimplePlayerFactory_hpp

#include "GamePre.H"
#include "SimpleActorFactory.hpp"

namespace game {

	class SimplePlayerFactory : public SimpleActorFactory {
	public:
		SimplePlayerFactory(se_core::String* name);
		~SimplePlayerFactory();

		se_core::Thing* create() const;
	};

}

#endif
