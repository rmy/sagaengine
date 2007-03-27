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


#ifndef AcStartEditor_hpp
#define AcStartEditor_hpp

#include "sim/sim.hpp"
#include "sim/action/Action.hpp"
#include "util/vecmath/ViewPoint.hpp"

namespace se_editor {
	class _SeEditorExport AcStartEditor : public se_core::Action {
	public:
		AcStartEditor();
		virtual ~AcStartEditor();

		short duration(se_core::ActionComponent& performer, se_core::Parameter& parameter) const { return 1; }
		bool isContinuing(se_core::ActionComponent &performer, se_core::Parameter& parameter) const { return false; }
		bool isRepeating(long when, se_core::ActionComponent &performer, se_core::Parameter& parameter) const { return false; }
		void perform(long when, se_core::ActionComponent& performer, se_core::Parameter& parameter) const;

	private:
	};

	extern _SeEditorExport const AcStartEditor actionStartEditor;
}

#endif
