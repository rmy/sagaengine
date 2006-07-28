#ifndef SimpleCameraFactory_hpp
#define SimpleCameraFactory_hpp

#include "GamePre.H"
#include "SimpleActorFactory.hpp"

namespace game {

	class SimpleCameraFactory : public SimpleActorFactory {
	public:
		SimpleCameraFactory(se_core::String* name);
		~SimpleCameraFactory();

		se_core::Thing* create() const;
	};

}

#endif
