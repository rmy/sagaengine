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


#ifndef ActionAndParameter_hpp
#define ActionAndParameter_hpp

#include "Action.hpp"
#include "../thing/sim_thing.hpp"
#include "util/type/util_type.hpp"
#include "util/error/Log.hpp"

namespace se_core {
	class _SeCoreExport Parameter {
	public:
		inline void* data(short size) {
			Assert(size < MAX_DATA_SIZE);
			return data_;
		}

		inline unsigned short actionStage() { return actionStage_; }
		void incrActionStage() { ++actionStage_; }
		void resetActionStage() { ++actionStage_ = 0; }

	private:
		static const short MAX_DATA_SIZE = (sizeof(coor_t) * 4 + sizeof(void*));
		unsigned short actionStage_;
		unsigned char data_[MAX_DATA_SIZE];
	};


	class _SeCoreExport ActionAndParameter {
	public:
		ActionAndParameter()
			: action_(0) {
		}

		~ActionAndParameter() {
		}


		inline void set(const ActionAndParameter& orig) {
			*this = orig;
		}

		inline bool hasAction() const {
			return action_ != 0;
		}

		inline const Action& setAction(const Action& a) {
			action_ = &a;
			parameter_.resetActionStage();
			return *action_;
		}


		inline void resetAction() {
			action_ = 0;
		}


		inline const Action* action() const {
			return action_;
		}


		inline void copyParameter(const Parameter& original) {
			Parameter& p = parameter();
			p = original;
		}


		inline Parameter& parameter() {
			return parameter_;
		}

		inline void* parameter(short size) {
			return parameter().data(size);
		}

	private:
		const Action* action_;
		Parameter parameter_;
	};

}

#endif
