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


#ifndef o3d_input_Console_hpp
#define o3d_input_Console_hpp

#include "O3dPre.hpp"
#include "OgreCEGUI.hpp"
#include "o3d_input.hpp"
#include <OIS/OIS.h>


namespace se_ogre {
	class _SeOgreExport Console {
	public:
		Console();
		~Console();


		void setGuiSystem();

		/**
		 * Set the console handler that will handle this
		 * object.
		 */
		void setConsoleHandler(ConsoleHandler* h);
		void setupGuiSystem();
		void createConsoleWindow();
		void initXMLMenu(const char* filename);

		/**
		 * Output message to console.
		 */
		void output(const char* msg);
		void output(const wchar_t* msg);


		void keyPressed(const OIS::KeyEvent* e);
		void mouseMoved (const OIS::MouseEvent *e);
		void mouseDragged (const OIS::MouseEvent *e);
		void mouseReleased (const OIS::MouseEvent *e, int button);
		void mousePressed (const OIS::MouseEvent *e, int button);


		bool isFocused() { return (isFocused_); /* && handler_); */ }
		void setFocus(bool newState) { isFocused_ = newState; }
		void flipFocus();
		void renderGui();
		void updateConsole();

	protected:
		void clearInput();

		bool isFocused_;
		ConsoleHandler* handler_;
		int inCount_, outCount_, lineCount_;
		static const int INPUT_BUFFER_SIZE = 256;
		wchar_t* input_;
		static const int OUTPUT_BUFFER_SIZE = 16384;
		wchar_t* output_;

#ifndef NO_CEGUI
		CEGUI::OgreCEGUIRenderer* guiRenderer_;
		CEGUI::System* guiSystem_;
		CEGUI::Window* editorGuiSheet_;
		CEGUI::MultiLineEditbox* editBox_;
#endif
		Ogre::RenderTexture* rttTexture_;
	};
}

#endif
