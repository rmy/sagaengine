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


#ifndef EditorAreaComponent_hpp
#define EditorAreaComponent_hpp

#include "sim/sim.hpp"
#include "comp/node/RootChildComponent.hpp"
#include "util/type/String.hpp"


namespace se_editor {
	class _SeEditorExport EditorAreaComponent : public se_core::RootChildComponent {
	public:
		typedef Ptr<EditorAreaComponent, se_core::sct_EDITOR> Ptr;
		
		/** Constructor.
		 */
		EditorAreaComponent(se_core::Composite* owner, const se_core::ComponentFactory* factory = 0);
		~EditorAreaComponent();
		void setActive(bool state);

		const char* name() { return "EditorArea"; }
		//se_core::String* grabString();

		void startEditor();

	protected:
		static const int MAX_STRINGS = 80;
		int usedStrings_;
		se_core::String strings_[ MAX_STRINGS ];
		friend class EditorAreaComponentFactory;
	};
}

#endif
