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


#ifndef EditorManager_hpp
#define EditorManager_hpp

#include "sim/sim.hpp"
#include "comp/node/RootComponent.hpp"


namespace se_editor {

	/**
	 * Base class for functionality component managers.
	 */
	class _SeEditorExport EditorManager : public se_core::RootComponent {
	public:
		/**
		 * Construct EditorManager.
		 */
		EditorManager();


		/**
		 * Destructor.
		 */
		virtual ~EditorManager();

		const char* name() { return "EditorManager"; }
		

		void step(long when);

		void initGame();
		void cleanupGame();

		void startEditor();
		void exitEditor();

		bool isEditing() const {
			return isEditing_;
		}

		static EditorManager& singleton();

	private:
		bool isEditing_;
	};

}

#endif
