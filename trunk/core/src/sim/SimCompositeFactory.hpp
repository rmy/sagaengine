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


#ifndef SimCompositeFactory_hpp
#define SimCompositeFactory_hpp

#include "sim.hpp"
#include "util/type/util_type.hpp"
#include "./config/sim_config.hpp"
#include "./script/sim_script.hpp"
#include "./stat/sim_stat.hpp"

namespace se_core {

	class _SeCoreExport SimCompositeFactory {
	public:
		SimCompositeFactory(short type, String* name);
		virtual ~SimCompositeFactory();
		const char* name() const;
		virtual SimComposite* create() const = 0;
		virtual void release(SimComposite* t);

		void setTag(int t) { tag_ = t; }
		int tag() { return tag_; }

	protected:
		String* name_;
		short type_;
		int tag_;
	};

}

#endif
