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


#ifndef engine_react_CoDefault_hpp
#define engine_react_CoDefault_hpp

#include "ThingCollide.hpp"

namespace se_core {
	class _SeCoreExport CoDefault : public ThingCollide {
	public:
		CoDefault();
		CoDefault(const char* name);
		bool collide(ContactInfo& pusher
					 , const ContactInfo& target) const;

		enum Substance { UNDEFINED, MOBILE, STATIC, GAS, PICK };
		const unsigned int PROPERTY_COLLIDE_SELF;

	protected:
		bool pop(ContactInfo& pusher, const ContactInfo& target) const;
		static bool _pop(se_core::Composite& spawner, unsigned int hash);
		bool popAndDie(ContactInfo& pusher, const ContactInfo& target) const;
		void bounce(ContactInfo& pusher, const ContactInfo& target) const;
		void rebound(ContactInfo& pusher, const ContactInfo& target) const;
		bool slide(ContactInfo& pusher, const ContactInfo& target) const;
		void _away(ContactInfo& pusher, const ContactInfo& target, float speed) const;

		void slideOrRebound(ContactInfo& pusher, const ContactInfo& target) const {
			if(!slide(pusher, target))
				rebound(pusher, target);
		}

		void slideOrBounce(ContactInfo& pusher, const ContactInfo& target) const {
			if(!slide(pusher, target))
				bounce(pusher, target);
		}
	};

	extern _SeCoreExport const CoDefault coDefault;
}

#endif
