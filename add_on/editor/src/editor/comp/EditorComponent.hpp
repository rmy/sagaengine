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


#ifndef EditorComponent_hpp
#define EditorComponent_hpp

#include "sim/sim.hpp"
#include "sim/AreaChildComponent.hpp"
#include "util/vecmath/ViewPoint.hpp"
#include "util/type/String.hpp"

namespace se_editor {
	class _SeEditorExport EditorComponent : public se_core::AreaChildComponent {
	public:
		typedef Ptr<EditorComponent, se_core::sct_EDITOR> Ptr;

		EditorComponent(se_core::SimComposite* owner, const se_core::SimComponentFactory* factory = 0);
		virtual ~EditorComponent();
		const char* name() { return "Editor"; }

	protected:
		friend class EditorComponentFactory;

		void cleanup();

		se_core::String* string_;
		se_core::ViewPoint start_;
		bool isGrounded_;
	};


}

#endif
