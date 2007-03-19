/*
SagaEngine library
Copyright (c) 2002-2006 Skalden Studio AS

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to distribute the library under the terms of the 
Q Public License version 1.0. Be sure to read and understand the license
before using the library. It should be included here, or you may read it
at http://www.trolltech.com/products/qt/licenses/licensing/qpl

The original version of this library can be located at:
http://www.sagaengine.com/

Rune Myrland
rune@skalden.com
*/


#ifndef Signal_hpp
#define Signal_hpp

#include "sim_signal.hpp"
#include "../SimComponent.hpp"
#include "comp/Composite.hpp"
#include "../action/ActionComponent.hpp"
#include "../action/ActionFeed.hpp"
#include "../action/sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/all.hpp"

namespace se_core {
	class _SeCoreExport Signal : public SimObject {
	public:
		Signal(const char* name);
		virtual ~Signal();

		virtual void recieve(SignalComponent& reciever, int id, bool state) const = 0;

	protected:
	};

}

#endif
