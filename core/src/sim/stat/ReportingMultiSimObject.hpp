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


#ifndef ReportingMultiSimObject_hpp
#define ReportingMultiSimObject_hpp

#include "MultiSimObject.hpp"
#include "sim_stat.hpp"

namespace se_core {
	class ReportingMultiSimObject : public MultiSimObject {
	public:
		ReportingMultiSimObject(SimObject& owner, short type);
		ReportingMultiSimObject(short type);
		virtual ~ReportingMultiSimObject();
		SimObject& owner() const { return *owner_; }
		short type() const { return type_; }
		void setHandler(MultiSimObjectListener* handler) { handler_ = handler; }
		void add(SimObject& value);
		void remove(SimObject& value);

	protected:
		short type_;
		SimObject* owner_;
		MultiSimObjectListener* handler_;
	};

}

#endif
