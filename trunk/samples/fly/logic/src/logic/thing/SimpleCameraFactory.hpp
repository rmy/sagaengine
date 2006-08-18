#ifndef SimpleCameraFactory_hpp
#define SimpleCameraFactory_hpp

#include "LogicPre.hpp"
#include "SimpleActorFactory.hpp"

namespace logic {

	class _FlyLogicExport SimpleCameraFactory : public SimpleActorFactory {
	public:
		SimpleCameraFactory(se_core::String* name);
		~SimpleCameraFactory();

		se_core::Thing* create() const;
	};

}

#endif