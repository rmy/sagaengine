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


#ifndef %sAreaComponent_hpp
#define %sAreaComponent_hpp

#include "comp/comp.hpp"
#include "comp/node/RootChildComponent.hpp"


namespace %n {
	class _%NExport %sAreaComponent : public se_core::RootChildComponent {
	public:
		typedef Ptr<%sAreaComponent, se_core::sct_%S> Ptr;
		
		/** Constructor.
		 */
		%sAreaComponent(se_core::Composite* owner, const se_core::ComponentFactory* factory = 0);
		~%sAreaComponent();
		void setActive(bool state);

		const char* name() { return "%sArea"; }

	protected:
		friend class %sAreaComponentFactory;
	};
}

#endif
