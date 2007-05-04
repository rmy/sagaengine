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


#ifndef ZoneComponent_hpp
#define ZoneComponent_hpp

#include "comp/comp.hpp"
#include "comp/node/AreaChildComponent.hpp"
#include "comp/list/ComponentList.hpp"
#include "util/vecmath/util_vecmath.hpp"
#include "sim/sim.hpp"
#include "Exit.hpp"


namespace se_core {

	class _SeCoreExport ZoneComponent : public AreaChildComponent {
	public:
		typedef ComponentPtr<ZoneComponent, sct_ZONE> Ptr;


		/** Constructor.
		 */
		ZoneComponent(Composite* owner, const ComponentFactory* factory = 0);
		~ZoneComponent();

		const char* name() { return owner()->name(); }
		int markerType() { return markerType_; }

	protected:
		friend class ZoneComponentFactory;

		int markerType_;
	};
}

#endif
