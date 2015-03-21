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


#include "CompositeFactory.hpp"
#include "Composite.hpp"
#include "ComponentFactory.hpp"
#include "util/type/String.hpp"
#include "util/error/Log.hpp"
#include <cstring>
#include <cstdio>

namespace se_core {
	CompositeFactory::Generic& CompositeFactory::gen() {
		static CompositeFactory::Generic g;
		return g;
	}

	CompositeFactory
	::CompositeFactory(CompositeType type, String* name)
		: name_(name), type_(type), tag_(0), componentCount_(0) {
	}


	CompositeFactory
	::~CompositeFactory() {
		while(componentCount_ > 0) {
			--componentCount_;
			if(components_[ componentCount_ ]->doDestroy()) {
				delete components_[ componentCount_ ];
			}
		}
		delete name_;
	}


	Composite* CompositeFactory
	::create() const {
		Composite* c = new Composite(this);
		createComponents(c);
		createGenericComponents(c);
		return c;
	}


	void CompositeFactory
	::release(Composite* t) const {
		delete t;
	}


	const char* CompositeFactory
	::name() const {
		return !name_ ? 0 : name_->get();
	}


	void CompositeFactory
	::addComponent(ComponentFactory* f) {
		Assert(componentCount_ < MAX_COMPONENTS);
		components_[ componentCount_++ ] = f;
	}


	void CompositeFactory
	::createComponents(Composite* owner) const {
		for(int i = 0; i < componentCount_; ++i) {
			components_[ i ]->create(owner);
		}
	}


	void CompositeFactory
	::createGenericComponents(Composite* owner) const {
		gen().createComponents(type_, owner);
	}

}
