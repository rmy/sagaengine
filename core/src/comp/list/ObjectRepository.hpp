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


#ifndef ObjectRepository_hpp
#define ObjectRepository_hpp

#include "comp/comp.hpp"

namespace se_core {
	class _SeCoreExport ObjectRepository {
	public:
		ObjectRepository(unsigned int size);
		virtual ~ObjectRepository();
		void add(const Object* go);
		void remove(const Object* go);
		const Object* get(int hash);
		bool has(int id);

	protected:
		unsigned int find(int hash);

		unsigned int objectCount_;
		const Object** objects_;
		unsigned int size_;
	};


}

#endif
