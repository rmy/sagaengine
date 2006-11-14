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


#ifndef MultiSimComposite_hpp
#define MultiSimComposite_hpp

#include "sim_stat.hpp"
#include "VoidList.hpp"
#include "../sim.hpp"
#include "../schema/SimSchema.hpp"

namespace se_core {

	class _SeCoreExport MultiSimComposite {
		friend class MultiSimCompositeIterator;

	public:
		typedef VoidList::iterator_type iterator_type;
		class _SeCoreExport Iterator {
		public:
			Iterator();
			Iterator(MultiSimComposite& mgo);
			void init(MultiSimComposite& mgo);
			void init(short firstNode);

			inline bool hasNext() {
				return it_ != VoidList::end();
			}

			inline SimComposite& next() {
				return *static_cast<SimComposite*>(SimSchema::voidList.next(it_));
			}

		private:
			VoidList::iterator_type it_;
		};


		MultiSimComposite();
		virtual ~MultiSimComposite();
		virtual void add(SimComposite& value);
		void add(MultiSimComposite& msc);
		virtual void remove(SimComposite& value);
		void initIterator(VoidList::iterator_type& iterator) const;
		inline VoidList::iterator_type iterator() const { return firstNode_; }
		bool contains(SimComposite& value) const;
		bool contains(MultiSimComposite& msc) const;
		bool sharesAny(MultiSimComposite& msc) const;
		bool isEmpty() const;
		void destroyMembersAndClear();
		void clear();
		int size() const;

	protected:
		VoidList::iterator_type firstNode_;
	};
}

#endif
