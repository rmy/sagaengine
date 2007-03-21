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


#ifndef %sComponent_hpp
#define %sComponent_hpp

#include "sim/sim.hpp"
#include "comp/comp.hpp"
#include "comp/node/AreaChildComponent.hpp"

namespace %n {
	class _%NExport %sComponent : public se_core::AreaChildComponent {
	public:
		typedef Ptr<%sComponent, se_core::sct_%S> Ptr;

		%sComponent(se_core::Composite* owner, const se_core::ComponentFactory* factory = 0);
		virtual ~%sComponent();
		const char* name() { return "%s"; }

	protected:
		friend class %sComponentFactory;

		void cleanup();
	};


}

#endif
