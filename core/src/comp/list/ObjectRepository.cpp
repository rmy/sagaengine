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


#include "ObjectRepository.hpp"
#include "comp/schema/CompSchema.hpp"
#include "comp/Object.hpp"
#include "util/error/Log.hpp"



namespace se_core {
	ObjectRepository
	::ObjectRepository(unsigned int size)
			: size_(size), objectCount_(0), objects_(new const Object*[ size ]) {
		LogDetail("Creating ObjectRepository");
	}


	ObjectRepository
	::~ObjectRepository() {
	}


	void ObjectRepository
	::add(const Object* go) {
		Assert(objectCount_ < size_ - 1);
		Assert(go != 0);

		unsigned int index = find(go->id());
		// Don't add to the list if it is there already...
		if(index == objectCount_ || objects_[index]->id() != go->id()) {
			for(unsigned int i = objectCount_; i > index; --i) {
				objects_[ i ] = objects_[ i - 1];
			}
			objects_[ index ] = go;
			++objectCount_;
		}
		else {
			LogFatal("Hash collision between " << *go << ") and " 
					<< *objects_[index] << ". Change the name of one of them.");
		}
		LogDetail("Registered Object: " << *go);
	}


	void ObjectRepository
	::remove(const Object* go) {
		unsigned int index = find(go->id());
		if(index == objectCount_ || objects_[ index ]->id() != go->id()) {
			return;
		}
		--objectCount_;
		for(unsigned int i = index; i < objectCount_; ++i) {
			objects_[ i ] = objects_[ i + 1];
		}
	}


	unsigned int ObjectRepository
	::find(int id) {
		unsigned int min = 0;
		unsigned int max = objectCount_;
		unsigned int middle;
		while(max > min) {
			middle = (max + min) >> 1;
			if(objects_[ middle ]->id() < id) {
				min = middle + 1;
			}
			else {
				max = middle;
			}
		}
		return min;
	}


	const Object* ObjectRepository
	::get(int id) {
		unsigned int index = find(id);
		Assert(index >= 0 && index <= objectCount_);
		Assert(objects_[ index ]->id() == id);

		return objects_[ index ];
	}


	bool ObjectRepository
	::has(int id) {
		unsigned int index = find(id);
		if(index == objectCount_ || objects_[ index ]->id() != id) {
			return false;
		}
		return true;
	}
}
