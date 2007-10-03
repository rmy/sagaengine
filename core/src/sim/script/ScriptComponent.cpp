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


#include "Script.hpp"
#include "ScriptComponent.hpp"
#include "../action/all.hpp"
#include "../thing/Actor.hpp"
#include "../schema/SimSchema.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"


namespace se_core {
	ScriptComponent
	::ScriptComponent(Composite* owner, ActionComponent* consumer)
		: Component(sct_SCRIPT, owner)
		, currentScript_(0)
		, consumer_(consumer) {
		scriptStack_[currentScript_] = 0;
		consumer_->setActionFeed(this);
	}


	ScriptComponent
	::ScriptComponent(Composite* owner, const ComponentFactory* factory)
			: Component(sct_SCRIPT, owner, factory)
			, currentScript_(0) {
		consumer_ = static_cast<ActionComponent*>(owner_->component(sct_ACTION));
		Assert(consumer_);
		scriptStack_[currentScript_] = 0;
		consumer_->setActionFeed(this);
	}

	ScriptComponent
	::~ScriptComponent() {
		cleanup();
	}

	void ScriptComponent
	::cleanup() {
		if(consumer_)
			consumer_->resetActionFeed();
		consumer_ = 0;
		clearScripts();
	}


	void ScriptComponent
	::nextAction(const ActionComponent& performer, int channel, ActionAndParameter& out) {
		nextScriptAction(channel, out);
	}


	void ScriptComponent
	::nextScriptAction(short channel, ActionAndParameter& aap) {
		if(!script()) return;
		//Parameter& p = aap.parameter();
		//const Action* a = script()->nextAction(*this, channel, scriptData(), p);
		//ActionAndParameter aap;
		script()->nextAction(*this, channel, scriptData(), aap);

		int s = currentScript_;
		while(!aap.hasAction() && s > 0 && scriptStack_[ s ]->isTransparent()) {
			--s;
			scriptStack_[ s ]->nextAction(*this, channel, scriptData_[ s ], aap);
		}

		/*
		if(aap.hasAction()) {
			//aap.setAction(*a);
			out.set(aap);
		}
		*/
	}


	void ScriptComponent
	::setDefaultScript(const Script* s) {
		if(!isActive() || currentScript_ != 0) {
			if(scriptStack_[0]) {
				scriptStack_[0]->release(scriptData_[0]);
			}
			scriptStack_[0] = s;
			scriptData_[0] = scriptStack_[0]->init(*this);
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		scriptStack_[0] = s;
		scriptData_[0] = scriptStack_[0]->init(*this);
		consumer_->setScriptActive(true);
	}


	void ScriptComponent
	::clearScripts() {
		// TODO:
		//if(showingCutscene_) {
		//	removeFromShowingCutscene();
		//}
		while(currentScript_ > 0) {
			scriptStack_[ currentScript_ ]->release( scriptData_[ currentScript_ ] );
			scriptStack_[ currentScript_-- ] = 0;
		}
		if(scriptStack_[0] != 0) {
			scriptStack_[ 0 ]->release( scriptData_[ 0 ] );
			scriptStack_[ 0 ] = 0;
		}
	}


	void ScriptComponent
	::pushScript(const char* name) {
		const Script* s = Script::lookup(name);
		pushScript(s);
	}


	void ScriptComponent
	::pushScript(const Script* s) {
		consumer_->setScriptActive(false);

		// If there already is a script running...
		if(script() && !script()->isStacker()) {
			// Release script data
			popScript();
		}

		if(scriptStack_[currentScript_] != 0) {
			++currentScript_;
		}

		Assert(currentScript_ < SCRIPT_STACK_SIZE);
		// Set the new script
		scriptStack_[currentScript_] = s;
		// Init script data
		scriptData_[currentScript_] = script()->init(*this);

		// If owner is active
		consumer_->setScriptActive(true);
	}


	void ScriptComponent
	::popScript() {
		// Asser(!isDead_);
		const char* popScript = script()->name();

		//bool isTransparent = script()->isTransparent();
		consumer_->setScriptActive(false);

		if(currentScript_ == 0) {
			Assert(script());
			script()->release(scriptData_[currentScript_]);
			scriptData_[currentScript_] = 0;
			scriptStack_[currentScript_] = 0;
			return;
		}

		if(script()) script()->release(scriptData_[currentScript_]);
		--currentScript_;
		if(!script()) return;

		script()->reinitPop(*this, scriptData(), popScript);

		if(isActive()) {
			consumer_->setScriptActive(true);
		}
	}


	void ScriptComponent
	::stopScript() {
		//TODO:
		//if(showingCutscene_) {
		//	removeFromShowingCutscene();
		//}
		//else {
		Assert(script());
		script()->release(scriptData_[currentScript_]);
		scriptData_[currentScript_] = 0;
		currentScript_ = 0;
		scriptStack_[0] = 0;
		//TODO:
		consumer_->disrupt();
		//}
	}


	void ScriptComponent
	::setActive(bool state) {
		// Setting to active??
		if(isActive()) {
			// Start script
			if(script()) {
				// Init (or reinit) scripts data block
				script()->reinit(*this, scriptData());
				// Start script in all action channels
				consumer_->setScriptActive(true);
			}
		}
	}


	void ScriptComponent
	::touch(void* param) {
		script()->touched(*this, scriptData(), param);
	}

	void ScriptComponent
	::touchDefault(void* param) {
		Assert(currentScript_ >= 0);
		const Script* s = scriptStack_[0];
		Assert(s);
		s->touched(*this, scriptData(), param);
	}


	void ScriptComponent
	::feedbackEvent(const ActionComponent& source, int type) {
		if(hasActiveScript()) {
			script()->feedbackEvent(*this, scriptData(), type);
		}
	}

}
