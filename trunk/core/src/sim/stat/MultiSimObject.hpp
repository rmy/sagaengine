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


#ifndef MultiSimObject_hpp
#define MultiSimObject_hpp

#include "sim_stat.hpp"
#include "SimObjectList.hpp"
#include "../sim.hpp"

namespace se_core {

	class _SeCoreExport MultiSimObject {
		friend class MultiSimObjectIterator;

	public:
		MultiSimObject();
		virtual ~MultiSimObject();
		virtual void add(SimObject& value);
		void add(MultiSimObject& mgo);
		virtual void remove(SimObject& value);
		void initIterator(SimObjectList::iterator_type& iterator) const;
		inline SimObjectList::iterator_type iterator() const { return firstNode_; }
		bool contains(SimObject& value) const;
		bool contains(MultiSimObject& mgo) const;
		bool sharesAny(MultiSimObject& mgo) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		SimObjectList::iterator_type firstNode_;
	};
}

#endif
