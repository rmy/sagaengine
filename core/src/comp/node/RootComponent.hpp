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


#ifndef RootComponent_hpp
#define RootComponent_hpp

#include "comp/comp.hpp"
#include "comp/node/NodeComponent.hpp"
#include "util/error/Log.hpp"

namespace se_core {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeCoreExport RootComponent : public NodeComponent {
	public:
		static Composite& activeRoot();
		static Composite& inactiveRoot();

		/**
		 * Construct RootComponent.
		 *
		 * @param type The type of RootComponent.
		 */
		RootComponent(int type);

		/**
		 * Destructor.
		 */
		virtual ~RootComponent() {
		}


		virtual void step(long when) = 0;

		virtual void init() { initGame(); }
		virtual void initGame() {}
		virtual void cleanupGame() {}
		virtual void cleanup() { cleanupGame(); }

	};

}

#endif
