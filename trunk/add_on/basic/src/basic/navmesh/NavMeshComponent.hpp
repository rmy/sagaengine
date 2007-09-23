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


#ifndef NavMeshComponent_hpp
#define NavMeshComponent_hpp

#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "comp/node/AreaChildComponent.hpp"

namespace se_basic {
	class _SeBasicExport NavMeshComponent : public se_core::AreaChildComponent {
	public:
		typedef ComponentPtr<NavMeshComponent, se_core::sct_NAVMESH> Ptr;

		NavMeshComponent(se_core::Composite* owner, const se_core::ComponentFactory* factory = 0);
		virtual ~NavMeshComponent();
		const char* name() const { return "NavMesh"; }

	protected:
		friend class NavMeshComponentFactory;

		void cleanup();
	};


}

#endif
