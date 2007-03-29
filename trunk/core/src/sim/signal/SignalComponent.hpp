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


#ifndef SignalComponent_hpp
#define SignalComponent_hpp

#include "Signal.hpp"
#include "comp/node/AreaChildComponent.hpp"
#include "comp/Composite.hpp"
#include "../action/ActionComponent.hpp"
#include "../action/ActionFeed.hpp"
#include "../action/sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/all.hpp"

namespace se_core {
	class _SeCoreExport SignalComponent : public AreaChildComponent {
	public:
		typedef Ptr<SignalComponent, sct_SIGNAL> Ptr;

		SignalComponent(Composite* owner, const ComponentFactory* factory = 0);
		virtual ~SignalComponent();
		const char* name() { return "Signal"; }

		static SignalComponent* get(Composite& composite) {
			SignalComponent* c = static_cast<SignalComponent*>(composite.component(se_core::sct_SIGNAL));
			return c;
		}

		static SignalComponent* get(Component& component) {
			SignalComponent* c = static_cast<SignalComponent*>(component.owner()->component(se_core::sct_SIGNAL));
			return c;
		}

		void setSendId(int id) {
			sendId_ = id;
		}

		void setRecieveId(int id, const Signal* signal) {
			recieveMask_ = (1 << id);
			signal_ = signal;
		}

		void setDead();

		bool send(bool state);
		long sentAge() const;
		void recieve(int id, bool state);
		void zoneChanged(int type, Composite* newArea, Composite* oldArea);


	protected:
		void cleanup();

		bool sendState_;
		int sendId_;
		long sentWhen_;
		unsigned long recieveMask_;
		const Signal* signal_;
	};


}

#endif
