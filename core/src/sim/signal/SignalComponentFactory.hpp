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


#ifndef SignalComponentFactory_hpp
#define SignalComponentFactory_hpp

#include "../sim.hpp"
#include "comp/ComponentFactory.hpp"
#include "comp/Composite.hpp"
#include "comp/ComponentFactory.hpp"
#include "util/type/util_type.hpp"
#include "sim_signal.hpp"

namespace se_core {
	class SignalComponentFactory : public ComponentFactory {
	public:
		SignalComponentFactory();

		virtual Component* create(Composite* owner) const;
		void setSendSignal(int id);
		void setRecieveSignal(unsigned long mask, const char* signal);

	protected:
		int signalSendId_;
		unsigned long signalRecieveMask_;
		const se_core::Signal* signal_;

	};

}

#endif
