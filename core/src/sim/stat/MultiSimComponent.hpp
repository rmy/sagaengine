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


#ifndef MultiSimComponent_hpp
#define MultiSimComponent_hpp

#include "sim_stat.hpp"
#include "SimComponentList.hpp"
#include "../sim.hpp"

namespace se_core {

	class _SeCoreExport MultiSimComponent {
		friend class MultiSimComponentIterator;

	public:
		typedef SimComponentList::iterator_type iterator_type;

		MultiSimComponent();
		virtual ~MultiSimComponent();
		virtual void add(SimComponent& value);
		void add(MultiSimComponent& msc);
		virtual void remove(SimComponent& value);
		void initIterator(SimComponentList::iterator_type& iterator) const;
		inline SimComponentList::iterator_type iterator() const { return firstNode_; }
		bool contains(SimComponent& value) const;
		bool contains(MultiSimComponent& msc) const;
		bool sharesAny(MultiSimComponent& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;
		inline SimComponent* next(iterator_type& iterator);

	protected:
		SimComponentList::iterator_type firstNode_;
	};
}

#endif
