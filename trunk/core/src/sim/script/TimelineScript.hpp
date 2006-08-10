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


#ifndef TimelineScript_hpp
#define TimelineScript_hpp

#include "Script.hpp"
#include "../action/sim_action.hpp"
#include "../thing/sim_thing.hpp"
#include "../../util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport TimelineScript : public Script {
	public:
		TimelineScript(String* name, int trackerPosCount = 40);
		virtual ~TimelineScript();
		virtual const Action* nextAction(const Actor& performer, int channel, ScriptData* sd, Parameter& out) const;
		void addAction(int trackerPos, const char* name);
		void addAction(int trackerPos, const Action* action);
		//virtual bool isAlwaysActive() { return false; }
		void addAction(int trackerPos, const Action* action, int parameter) {
			addAction(trackerPos, action);
			setParameter(trackerPos, parameter);
		}
		void addAction(int trackerPos, const char* name, int parameter) {
			addAction(trackerPos, name);
			setParameter(trackerPos, parameter);
		}
		void addAction(int trackerPos, const char* name, String* attribute) {
			addAction(trackerPos, name);
			setAttribute(trackerPos, attribute);
		}
		void addAction(int trackerPos, const Action* action, String* attribute) {
			addAction(trackerPos, action);
			setAttribute(trackerPos, attribute);
		}
		void addAction(int trackerPos, const Action* action, String* attribute, int parameter) {
			addAction(trackerPos, action);
			setAttribute(trackerPos, attribute);
			setParameter(trackerPos, parameter);
		}
		void addAction(int trackerPos, const char* name, String* attribute, int parameter) {
			addAction(trackerPos, name);
			setAttribute(trackerPos, attribute);
			setParameter(trackerPos, parameter);
		}

		//virtual void activate(Actor& performer) const {}
		void setZeroMode(bool state) { isInZeroMode_ = state; }

	private:
		void setParameter(int trackerPos, int value) { scriptSingleValueParameters_[ trackerPos ] = value; }
		void setAttribute(int trackerPos, String* value) { scriptAttributeParameters_[ trackerPos ] = value; }

		short trackerPosCount_;
		const Action** actions_;
		int* scriptSingleValueParameters_;
		String** scriptAttributeParameters_;
		String* nameString_; // For proper destruction of name strings
		bool isInZeroMode_;
	};

}

#endif
