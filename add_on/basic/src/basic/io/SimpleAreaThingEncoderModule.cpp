#include "SimpleAreaThingEncoderModule.hpp"
#include "comp/list/CompositeList.hpp"
#include "io/stream/OutputStream.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/area/Area.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/spawn/SpawnComponent.hpp"
#include <cstring>

using namespace se_core;


namespace se_basic {
	SimpleAreaThingEncoderModule
	::SimpleAreaThingEncoderModule(Encoder& encoder)
			: EncoderModule(encoder, ParserModule::GAME, ParserModule::AREA_THINGS, 1)  {
	}


	SimpleAreaThingEncoderModule
	::~SimpleAreaThingEncoderModule() {
	}


	void SimpleAreaThingEncoderModule
	::encode(OutputStream& out) {
		out.writeHeaderCode(headerCode());

		for(int i = 0; i < SimSchema::areaManager.areaCount(); ++i) {
			const Area* a = SimSchema::areaManager.area(i);
			out.writeInfoCode('N');
			out.writeString(a->name());

			const SpawnComponent* pSpawn = SpawnComponent::get(*a);
			for(int j = 0; j < pSpawn->spawnPointCount(); ++j) {
				const ViewPoint* sp = pSpawn->spawnPoint(j);
				if(sp) {
					out.writeInfoCode('E');
					out.writeShort(j);
					out.writeInfoCode('T');
					out.writeFloat(sp->coor_.x_);
					out.writeFloat(sp->coor_.y_);
					out.writeFloat(sp->coor_.z_);
					out.writeInfoCode('R');
					out.writeFloat(BrayT::toDeg(sp->face_.yaw_));
					out.writeFloat(BrayT::toDeg(sp->face_.pitch_));
					out.writeFloat(BrayT::toDeg(sp->face_.roll_));
					out.writeInfoCode('/');
				}
			}

			const PosComponent* aPos = PosComponent::get(*a);

			CompositeList::Iterator it(a->children());
			while(it.hasNext()) {
				Composite& t = it.next();
				out.writeInfoCode('A');
				out.writeString(t.name());
				PosComponent* pPos = PosComponent::get(t);
				if(pPos->pos().isGrounded()) {
					out.writeInfoCode('G');
				}
				ViewPoint vp(pPos->pos().worldViewPoint());
				vp.sub(aPos->pos().worldViewPoint());
				out.writeInfoCode('T');
				out.writeFloat(vp.coor_.x_);
				out.writeFloat(vp.coor_.y_);
				out.writeFloat(vp.coor_.z_);
				out.writeInfoCode('R');
				out.writeFloat(BrayT::toDeg(vp.face_.yaw_));
				out.writeFloat(BrayT::toDeg(vp.face_.pitch_));
				out.writeFloat(BrayT::toDeg(vp.face_.roll_));
				out.writeInfoCode('/');
			}

		}
		out.writeInfoCode('Q');
	}

}
