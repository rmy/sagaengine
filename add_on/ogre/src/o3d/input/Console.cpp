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


#include "O3dPre.hpp"
#include "Console.hpp"
#include "ConsoleHandler.hpp"
#include "util/error/Log.hpp"
#include "../schema/O3dSchema.hpp"
#include <cstring>
#ifndef NO_CEGUI
#include <CEGUI/CEGUIFontManager.h>
#endif

namespace se_ogre {
#ifndef NO_CEGUI
	CEGUI::MouseButton convertOgreButtonToCegui(int buttonID) {
		switch (buttonID) {
		case Ogre::MouseEvent::BUTTON0_MASK:
			return CEGUI::LeftButton;
		case Ogre::MouseEvent::BUTTON1_MASK:
			return CEGUI::RightButton;
		case Ogre::MouseEvent::BUTTON2_MASK:
			return CEGUI::MiddleButton;
		case Ogre::MouseEvent::BUTTON3_MASK:
			return CEGUI::X1Button;
		default:
			return CEGUI::LeftButton;
	   }
	}
#endif

	Console
	::Console()
		: isFocused_(false), handler_(0)
		, inCount_(0), outCount_(0)
#ifndef NO_CEGUI
		, guiRenderer_(0), guiSystem_(0), editorGuiSheet_(0)
#endif
	{
		output_[ outCount_ ] = 0;
		clearInput();
	}


	Console
	::~Console() {
#ifndef NO_CEGUI

		if(editorGuiSheet_) {
			CEGUI::WindowManager::getSingleton().destroyWindow(editorGuiSheet_);
		}
		if(guiSystem_) {
			delete guiSystem_;
			guiSystem_ = 0;
		}
		if(guiRenderer_) {
			delete guiRenderer_;
			guiRenderer_ = 0;
		}
#endif
	}


	void Console
	::clearInput() {
		inCount_ = 0;
		input_[ inCount_++ ] = '>';
		input_[ inCount_++ ] = ' ';
		input_[ inCount_ ] = 0;
	}


	void Console
	::flipFocus() {
#ifndef NO_CEGUI
		isFocused_ = !isFocused_;
		if(isFocused()) {
			editBox_->show();
			editBox_->activate();
		}
		else {
 			editBox_->deactivate();
			editBox_->hide();
		}
#endif
	}


	void Console
	::updateConsole() {
#ifndef NO_CEGUI
		// Text is output text with current input line appended
		CEGUI::String s;
		s = output_;
		s += input_;
		// Show text
		editBox_->setText(s);
		// Scroll to bottom
		editBox_->ensureCaratIsVisible();
		editBox_->setCaratIndex(s.length());
#endif
	}


	void Console
	::setupGuiSystem() {
#ifndef NO_CEGUI
		// setup GUI system
		guiRenderer_ = new CEGUI::OgreCEGUIRenderer(O3dSchema::window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, O3dSchema::sceneManager);
		guiSystem_ = new CEGUI::System(guiRenderer_);
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
		LogMsg("Initialized CEGUI logger");

		/*
		// Setup Render To Texture for preview window
		rttTexture_ = Ogre::Root::getSingleton().getRenderSystem()->createRenderTexture( "ceguiTex", 512, 512, Ogre::TEX_TYPE_2D, Ogre::PF_A8R8G8B8 );
		LogMsg("Created CEGUI render texture");

		// Update when needed
		rttTexture_->setActive(false);
		rttTexture_->setAutoUpdated(false);
		*/

		/*
		{
			Ogre::Camera* rttCam = O3dSchema::sceneManager->createCamera("CeguiCam");
			Ogre::SceneNode* camNode = O3dSchema::sceneManager->getSceneNode("MainSceneNode")->createChildSceneNode("ceguiCamNode");
			camNode->attachObject(rttCam);
			rttCam->setPosition(0,0,200);
			//rttCam->setVisible(false);
			Ogre::Viewport *v = rttTexture_->addViewport( rttCam );
			v->setOverlaysEnabled(false);
			v->setClearEveryFrame(true);
			v->setBackgroundColour(Ogre::ColourValue::Black);
		}
		*/
		{
			CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme");
			guiSystem_->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
			CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseMoveCursor");
			guiSystem_->setDefaultFont((CEGUI::utf8*)"Tahoma-12");
		}
		/*
		LogMsg("Created CEGUI viewport");

		// Retrieve CEGUI texture for the RTT
		CEGUI::Texture* ceguiTexture = guiRenderer_->createTexture((CEGUI::utf8*)"ceguiTex");
		LogMsg("Created CEGUI texture");

		CEGUI::Imageset* rttImageSet = CEGUI::ImagesetManager::getSingleton().createImageset((CEGUI::utf8*)"ceguiImageset", ceguiTexture);
		rttImageSet->defineImage((CEGUI::utf8*)"ceguiImage", CEGUI::Point(0.0f, 0.0f), CEGUI::Size(ceguiTexture->getWidth(), ceguiTexture->getHeight()), CEGUI::Point(0.0f,0.0f));

		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLook.scheme");
		guiSystem_->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");

		// selecting the font
		//CEGUI::Font *font = CEGUI::FontManager::getSingleton().createFont((CEGUI::utf8*)"verdana.font");
		//guiSystem_->setDefaultFont(font);
		guiSystem_->setDefaultFont((CEGUI::utf8*)"Tahoma-12");
		*/

#endif
	}

	void Console
	::renderGui() {
#ifndef NO_CEGUI
		guiSystem_->renderGUI();
#endif
	}


	void Console
	::createConsoleWindow() {
#ifndef NO_CEGUI
		if(!editorGuiSheet_) {
			editorGuiSheet_ = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultWindow", (CEGUI::utf8*)"Sheet");
			guiSystem_->setGUISheet(editorGuiSheet_);
		}

		//////////////////////////////////////////
		editBox_ = (CEGUI::MultiLineEditbox*)CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/MultiLineEditbox", (CEGUI::utf8*)"Console");
		editorGuiSheet_->addChildWindow(editBox_);
		editBox_->setPosition(CEGUI::Point(0.0f, 0.0f));
		editBox_->setSize(CEGUI::Size(0.3f, 0.6f));

		// TODO: Do manual update on the RenderTexture, to avoid performance drop. (from wumpus)
		updateConsole();
		if(isFocused()) {
			editBox_->activate();
		}
		else {
			editBox_->hide();
		}
#endif
	}


	void Console
	::initXMLMenu(const char* filename) {
#ifndef NO_CEGUI
		editorGuiSheet_ = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)filename);
		guiSystem_->setGUISheet(editorGuiSheet_);
#endif
	}


	void Console
	::setConsoleHandler(ConsoleHandler* h) {
		// Cannot reset handler via this method
		Assert(h);

		// The handler should not be registered with
		// another console
		Assert(!h->console_);

		// Reset replaced handler
		if(handler_) {
			handler_->console_ = 0;
		}

		// Register new handler
		handler_ = h;
		// Register console with new handler
		h->console_ = this;
	}


	void Console
	::output(const char* msg) {
		const char* src = msg;

		int len = (int)strlen(msg);
		if(outCount_ + len >= OUTPUT_BUFFER_SIZE - 1) {
			for(int i = 0; i < outCount_ - len; ++i) {
				output_[i] = output_[ i + len ];
			}
			outCount_ -= len;
		}

		while(*src != 0 && outCount_ < OUTPUT_BUFFER_SIZE) {
			output_[ outCount_++ ] = *(src++);
		}

	}


	void Console
	::keyPressed(Ogre::KeyEvent* e) {
#ifndef NO_CEGUI
		if(editBox_->hasInputFocus()) {
			switch(e->getKey()) {
			case Ogre::KC_RETURN:
				output(input_);
				output("\n");
				if(handler_) handler_->parseCommand(&input_[2]);
				clearInput();
				break;

			case Ogre::KC_UP:
			case Ogre::KC_DOWN:
			case Ogre::KC_PGUP:
			case Ogre::KC_PGDOWN:
				CEGUI::System::getSingleton().injectKeyDown(e->getKey());
				CEGUI::System::getSingleton().injectChar(e->getKeyChar());
				// Don't update console after cursor keys,
				// because doing so moves the cursor back to the
				// end of the document
				return;

			case Ogre::KC_BACK:
				if(inCount_ > 2) {
					input_[ --inCount_ ] = 0;
				}
				break;
			}

			char ch = e->getKeyChar();
			if(ch && inCount_ < INPUT_BUFFER_SIZE - 1) {
				input_[ inCount_++ ] = ch;
				input_[ inCount_ ] = 0;
				LogMsg("Key: " << inCount_ << " (" << (int)ch << ") " << input_);
			}
			updateConsole();
		}
		else {
			CEGUI::System::getSingleton().injectKeyDown(e->getKey());
			CEGUI::System::getSingleton().injectChar(e->getKeyChar());
		}
#endif
	}


	void Console
	::mouseMoved (Ogre::MouseEvent *e) {
#ifndef NO_CEGUI
		CEGUI::System::getSingleton().injectMouseMove(
			e->getRelX() * guiRenderer_->getWidth(),
			e->getRelY() * guiRenderer_->getHeight());
		e->consume();
#endif
	}


	void Console
	::mouseDragged (Ogre::MouseEvent *e) {
		mouseMoved(e);
	}


	void Console
	::mousePressed (Ogre::MouseEvent *e) {
#ifndef NO_CEGUI
		CEGUI::System::getSingleton().injectMouseButtonDown(
				convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
#endif
	}


	void Console
	::mouseReleased (Ogre::MouseEvent *e) {
#ifndef NO_CEGUI
		CEGUI::System::getSingleton().injectMouseButtonUp(
				convertOgreButtonToCegui(e->getButtonID()));
		e->consume();
#endif
	}

}
