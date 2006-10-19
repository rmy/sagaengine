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


#ifndef SimCompositeOwner_hpp
#define SimCompositeOwner_hpp

#include "SimObject.hpp"
#include "util/template/HashTable.hpp"


namespace se_core {
	typedef HashTable<class SimComposite, 200> SCOHT;
	class _SeCoreExport Composites : public SCOHT  {
	};


	class _SeCoreExport SimCompositeOwner : public SimObject {
	protected:
		SimCompositeOwner(enum SimObjectType type, const char* name);

	public:
		SimCompositeOwner(const char* name);
		SimComposite* composite(int type) {
			return composites_.lookup(type);
		}

		bool isActive() { return isActive_; }
		bool isDead() { return isDead_; }
		void setActive(bool state) {
			if(state == isActive_) return;

			// TODO: 
			// Traverse children
			//   warn active
			isActive_ = state;
		}

	private:
		Composites composites_;
		bool isActive_;
		bool isDead_;
	};

}


#endif
