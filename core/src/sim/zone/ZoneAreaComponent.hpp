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


#ifndef ZoneAreaComponent_hpp
#define ZoneAreaComponent_hpp

#include "comp/comp.hpp"
#include "comp/node/RootChildComponent.hpp"
#include "comp/list/ComponentList.hpp"
#include "util/vecmath/util_vecmath.hpp"
#include "sim/sim.hpp"
#include "Exit.hpp"


namespace se_core {
	class _SeCoreExport Page {
	public:
		int x_, y_, z_, w_;

		Page() {}
		Page(int x, int y, int z, int w) : x_(x), y_(y), z_(z), w_(w) {}
		Page(const Page& p1, const Page& p2) 
				: x_(p1.x_ - p2.x_), y_(p1.y_ - p2.y_), z_(p1.z_ - p2.z_), w_(p1.w_ - p2.w_) {
		}


		void set(int x, int y, int z, int w) {
			x_ = x;
			y_ = y;
			z_ = z;
			w_ = w;
		}


		void sub(const Page& p1, const Page& p2) {
			x_ = p1.x_ - p2.x_;
			y_ = p1.y_ - p2.y_;
			z_ = p1.z_ - p2.z_;
			w_ = p1.w_ - p2.w_;
		}


		bool isNeighbourOffset() const {
			return (x_ >= -1 && x_ <= 1
				&& y_ >= -1 && y_ <= 1
				&& z_ >= -1 && z_ <= 1
				&& w_ == 0);
		}


		void clampToNeighbourOffset(const Page& rel) {
			x_ = rel.x_;
			y_ = rel.y_;
			z_ = rel.z_;
			w_ = rel.w_;

			if(x_ < -1) x_ = -1;
			if(x_ > 1) x_ = 1;

			if(y_ < -1) z_ = -1;
			if(y_ > 1) z_ = 1;

			if(z_ < -1) z_ = -1;
			if(z_ > 1) z_ = 1;
		}
	};


	class _SeCoreExport ZoneAreaComponent : public RootChildComponent {
	public:
		typedef ComponentPtr<ZoneAreaComponent, sct_ZONE> Ptr;



		/** Constructor.
		 */
		ZoneAreaComponent(Composite* owner, const ComponentFactory* factory = 0);
		~ZoneAreaComponent();
		void setActive(bool state);

		const char* name() { return "ZoneArea"; }

		void offset(const ZoneAreaComponent& other, Page& dest) const;
		bool offset(const ZoneAreaComponent& other, const Vector3& size, Vector3& dest) const;

		Page& page() { return page_; }
		const Page& page() const { return page_; }

		bool addNeighbour(ZoneAreaComponent& other);
		int neighbourIndex(const Page& rel) const;
		ZoneAreaComponent* neighbour(int relX, int relY, int relZ);
		const ZoneAreaComponent* neighbour(int relX, int relY, int relZ) const;

		void addLink(ZoneAreaComponent& link);
		ComponentList& links() { return links_; }
		void setExits(Exit* exits, int count);
		Exit& exit(int id);
		const Exit& exit(int id) const;
		int exitCount() const { return exitCount_; }

	protected:
		friend class ZoneAreaComponentFactory;

		Page page_;
		ComponentList links_;
		static const int MAX_NEIGHBOURS = 3 * 3 * 3;
		ZoneAreaComponent* neighbours_[ MAX_NEIGHBOURS ];
		int exitCount_;
		Exit* exits_;
	};
}

#endif
