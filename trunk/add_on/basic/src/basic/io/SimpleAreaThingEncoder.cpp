#include "SimpleAreaThingEncoder.hpp"
#include "comp/list/CompositeList.hpp"
#include "io/stream/OutputStream.hpp"
#include "sim/area/AreaManager.hpp"
#include "sim/area/Area.hpp"
#include "sim/pos/PosComponent.hpp"
#include "sim/spawn/SpawnComponent.hpp"
#include "sim/zone/ZoneAreaComponent.hpp"
#include "sim/custom/StatComponent.hpp"
#include "sim/signal/SignalComponent.hpp"
#include "io/parse/Parser.hpp"
#include <cstring>

using namespace se_core;


namespace se_basic {

	SimpleAreaThingEncoder
	::SimpleAreaThingEncoder(EncodeManager& encoder)
			: Encoder(encoder, Parser::GAME, Parser::AREA_THINGS, 1)  {
	}


	SimpleAreaThingEncoder
	::~SimpleAreaThingEncoder() {
	}


	void SimpleAreaThingEncoder
	::encode(OutputStream& out) {

		for(int i = 0; i < SimSchema::areaManager.areaCount(); ++i) {
			out.writeHeaderCode(headerCode());
			const Area* a = SimSchema::areaManager.area(i);
			out.writeInfoCode('N');
			out.writeString(a->owner()->name());

			const SpawnComponent* pSpawn = SpawnComponent::get(*a);
			for(int j = 0; j < pSpawn->spawnPointCount(); ++j) {
				if(pSpawn->hasSpawnPoint(j)) {
					const ViewPoint* sp = pSpawn->spawnPoint(j);
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

			const ZoneAreaComponent::Ptr pZone(*a);
			Assert(!pZone.isNull());
			for(int j = 0; j < pZone->exitCount(); ++j) {
				const Exit& exit = pZone->exit(j);
				out.writeInfoCode('X');
				out.writeString(exit.area_->get());
				out.writeShort(exit.entrance_);
			}

			const PosComponent* aPos = PosComponent::get(*a);

			CompositeList::Iterator it(a->owner()->children());
			while(it.hasNext()) {
				Composite& t = it.next();
				if(!StatComponent::Ptr(t)->shouldSave())
					continue;
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

				for(int i = 0; i < 4; ++i) {
					const Anim& a = pPos->pos().anim(i);
					out.writeInfoCode('A');
					out.writeShort(i);
					out.writeShort(a.movementMode());
					out.writeFloat(a.startPos());
					out.writeFloat(a.pos());
					out.writeFloat(a.speed());
					out.writeFloat(a.weight());
				}

				SignalComponent::Ptr signal(t);
				if(!signal.isNull() && signal->isOn()) {
					bool state = signal->isOn();
					out.writeInfoCode('I');
					// TODO: ??? Test this
					//out.writeShort(state ? 1 : 0);
					out.writeShort(0);
				}

				out.writeInfoCode('/');
			}

			out.writeInfoCode('Q');
		}
	}

}
