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


#ifndef CameraComponent_hpp
#define CameraComponent_hpp

#include "comp/Component.hpp"
#include "sim/pos/PosComponent.hpp"

namespace se_client {
	class _SeCoreExport CameraComponent : public se_core::Component {
	public:
		typedef se_core::Component::ComponentPtr<CameraComponent, se_core::sct_CAMERA> Ptr;

		CameraComponent(se_core::Composite* owner);
		virtual ~CameraComponent();

		void setActive(bool state);
		void zoneChanged(int zoneType, se_core::Composite* newArea, se_core::Composite* oldArea);
		void grabFocus();

		void setDoSee(bool flag) { doesSee_ = flag; }
		bool doesSee() const { return doesSee_; }

	protected:
		se_core::PosComponent* posComponent_;

		bool doesSee_;
	};

}

#endif
