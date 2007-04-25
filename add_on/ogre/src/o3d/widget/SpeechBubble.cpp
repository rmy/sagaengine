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
	::SpeechBubble() : speaker_(0), overlay_(0), caption_(0) {
	}


	SpeechBubble
	::~SpeechBubble() {
	}


	bool SpeechBubble
	::init() {
		overlay_ = Ogre::OverlayManager::getSingleton().getByName("Bubbins/SpeechBubble");
		caption_ = Ogre::OverlayManager::getSingleton().getOverlayElement("Bubbins/SpeechBubbleText");

		SimSchema::messageCentral.addListener(*this);
		return true;
	}


	void SpeechBubble
	::cleanup() {
		SimSchema::messageCentral.removeListener(*this);
		speaker_ = 0;
	}


	void SpeechBubble
	::infoEvent(char* text) {
		LogDetail(text);
	}


	void SpeechBubble
	::speechEvent(se_core::Actor& speaker, const char* messageName) {
		speaker_ = &speaker;
		const char* message = ClientSchema::phrases.getPhrase(Phrase::SPEECH, messageName);
		char buffer[256];
		const char* c = message;
		int linePos = 0;
		int i = 0;
		for(; *c != 0 && i < 255; ++i, ++c) {
			buffer[i] = *c;
			++linePos;
			if(*c == '_') {
				if(linePos < 50) {
					buffer[i] = ' ';
				}
				else {
					buffer[i] = '\n';
					linePos = 0;
				}
			}
		}
		buffer[i] = 0;

		LogDetail(speaker.name() << " says '" << message << "'");

		try {
			caption_->setCaption(buffer);
			overlay_->show();
		}
		catch(...) {
			LogWarning("Couldn't show speech bubble");
		}
	}


	void SpeechBubble
	::trackUserFeedback() {
		if(speaker_)
			speaker_->script()->trackUserFeedback();
		overlay_->hide();
	}

}

