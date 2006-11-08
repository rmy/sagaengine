#ifndef SimpleCameraFactory_hpp
#define SimpleCameraFactory_hpp

#include "BasicPre.hpp"
#include "SimpleActorFactory.hpp"

namespace se_basic {

	class _SeBasicExport SimpleCameraFactory : public SimpleActorFactory {
	public:
		SimpleCameraFactory(se_core::String* name);
		~SimpleCameraFactory();

		se_core::SimComposite* create() const;
	};

}

#endif
