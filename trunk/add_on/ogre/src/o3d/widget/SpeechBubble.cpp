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
#include "SpeechBubble.hpp"
#include "../schema/O3dSchema.hpp"
#include "sim/message/MessageCentral.hpp"
#include "client/lang/Phrase.hpp"
#include "sim/thing/Actor.hpp"
#include "sim/script/Script.hpp"
#include <OgreRenderWindow.h>
#include <OgreOverlayManager.h>


using namespace se_client;
using namespace se_core;

namespace se_ogre {
	SpeechBubble
	::SpeechBubble() : InputHandler("SpeechBubble"), speaker_(0), speechOverlay_(0), infoOverlay_(0), speechCaption_(0), infoCaption_(0) {
	}


	SpeechBubble
	::~SpeechBubble() {
	}


	bool SpeechBubble
	::init() {
		try {
			speechOverlay_ = Ogre::OverlayManager::getSingleton().getByName("Bubbins/SpeechBubble");
			speechCaption_ = Ogre::OverlayManager::getSingleton().getOverlayElement("Bubbins/SpeechBubbleText");
		}
		catch(...) {
			LogWarning("Failed setting upp speech bubble overlays.");
		}

		try {
			infoOverlay_ = Ogre::OverlayManager::getSingleton().getByName("Bubbins/InfoBubble");
			infoCaption_ = Ogre::OverlayManager::getSingleton().getOverlayElement("Bubbins/InfoBubbleText");
		}
		catch(...) {
			LogWarning("Failed setting upp info bubble overlays.");
		}

		SimSchema::messageCentral.addListener(*this);
		return true;
	}


	void SpeechBubble
	::cleanup() {
		SimSchema::messageCentral.removeListener(*this);
		speaker_ = 0;
	}


	void SpeechBubble
	::infoEvent(char* messageName) {
		if(!messageName) {
			try {
				infoOverlay_->hide();
			}
			catch(...) {
				LogWarning("Couldn't hide info bubble");
			}
			return;
		}

		wchar_t buffer[512];
		LogDetail("Info: " << messageName);
		bool hadTranslation = translate(messageName, buffer);
		try {
			if(hadTranslation)
				infoCaption_->setCaption(buffer);
			else
				infoCaption_->setCaption(messageName);
			infoOverlay_->show();
		}
		catch(...) {
			LogWarning("Couldn't show info bubble");
		}
	}


	bool SpeechBubble
	::translate(const char* messageName, wchar_t* buffer) {
		const char* message = ClientSchema::phrases().getPhrase(Phrase::SPEECH, messageName);

		if(!message)
			return false;
		const unsigned char* c = (const unsigned char*)message;
		int linePos = 0;
		int i = 0;
		for(; *c != 0 && i < 255; ++i, ++c) {
			buffer[i] = *c;
			++linePos;
			if(*c == '_') {
				if(linePos < 45) {
					buffer[i] = ' ';
				}
				else {
					buffer[i] = '\n';
					linePos = 0;
				}
			}
		}
		buffer[i] = 0;
		return true;
	}


	void SpeechBubble
	::speechEvent(se_core::Actor& speaker, const char* messageName) {
		LogDetail(speaker.name() << " says '" << messageName << "'");
		speaker_ = &speaker;
		wchar_t buffer[512];
		bool hadTranslation = translate(messageName, buffer);
		try {
			if(hadTranslation)
				speechCaption_->setCaption(buffer);
			else
				speechCaption_->setCaption(messageName);
			speechOverlay_->show();
		}
		catch(...) {
			LogWarning("Couldn't show speech bubble");
		}

		grabFocus();
	}


	void SpeechBubble
	::trackUserFeedback() {
		if(speaker_)
			speaker_->script()->trackUserFeedback();
		speechOverlay_->hide();

		loseFocus();
	}

}

