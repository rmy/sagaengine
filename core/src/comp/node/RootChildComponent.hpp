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


#ifndef comp_RootChildComponent_hpp
#define comp_RootChildComponent_hpp

#include "comp/node/NodeComponent.hpp"


namespace se_core {
	/** Compnent that registers itself as a child of CompSchema::activeRoot() nodes when active.
	 */
	class _SeCoreExport RootChildComponent : public NodeComponent {
	public:
		RootChildComponent(int type, Composite* owner, const ComponentFactory* factory = 0);
		virtual ~RootChildComponent();

	protected:
		void setActive(bool state);
	};

}

#endif
