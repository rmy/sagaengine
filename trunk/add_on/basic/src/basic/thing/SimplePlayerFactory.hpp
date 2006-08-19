#ifndef SimplePlayerFactory_hpp
#define SimplePlayerFactory_hpp

#include "BasicPre.hpp"
#include "SimpleActorFactory.hpp"

namespace se_basic {

	class _SeBasicExport SimplePlayerFactory : public SimpleActorFactory {
	public:
		SimplePlayerFactory(se_core::String* name);
		~SimplePlayerFactory();

		se_core::Thing* create() const;
	};

}

#endif
