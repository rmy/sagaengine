#include "NavMesh.hpp"

using namespace se_core;

namespace se_basic {
	// isLeft(): tests if point P2 is Left|On|Right of the line P0 to P1.
	//      returns: >0 for left, 0 for on, and <0 for right of the line.
	//      (see the January 2001 Algorithm on Area of Triangles)
	float left(const Point3& P0, const Point3& P1, const Point3& P2 ) {
		return (P1.x_ - P0.x_) * (P2.z_ - P0.z_) - (P2.x_ - P0.x_) * (P1.z_ - P0.z_);
	}


	bool doLinesIntersectXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1) {
		float s1 = left( b0, b1, a0) * left( b0, b1, a1);
		float s2 = left( a0, a1, b0) * left( a0, a1, b1);
		//LogMsg("Side: " << s1 << " - " << s2);
		return (s1 < 0 && s2 < 0);
	}

	bool doLinesIntersectXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1
						  , se_core::Point2* out) {
		coor_t ua =
			((b1.x_ - b0.x_) * (b0.z_ - a0.z_)
			 - (b1.z_ - b0.z_) * (b0.x_ - a0.x_))
			/
			((a1.z_ - a0.z_) * (b1.x_ - b0.x_)
			 - (a1.x_ - a0.x_) * (b1.z_ - b0.z_));


		coor_t ub =
			((a1.x_ - a0.x_) * (a0.z_ - b0.z_)
			 - (a1.z_ - a0.z_) * (a0.x_ - b0.x_))
			/
			((b1.z_ - b0.z_) * (a1.x_ - a0.x_)
			 - (b1.x_ - b0.x_) * (a1.z_ - a0.z_));

		if(out) {
			out->x_ = a0.x_ + ua * (a1.x_ - a0.x_);
			out->y_ = a0.z_ + ua * (a1.z_ - a0.z_);
		}
		//if(ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
		//LogMsg(ua << ", " << ub << ": " << a0.x_ + ua * (a1.x_ - a0.x_) << ", " << a0.z_ + ua * (a1.z_ - a0.z_) << ": " << (doLinesIntersectXZ(a0, a1, b0, b1) ? "true" : "false") );
		return(ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1);
	}


	bool NavMesh
	::isInLineOfSight(const se_core::Pos& from, const se_core::Pos& to) const {
		static const short corners[][2] =
			{
				{ 1, 2 },
				{ 2, 0 },
				{ 0, 1 }
			};

		short via = from.index();
		short prev = -1;
		while(via != to.index()) {
			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			short next = -1;
			for(int i = 0; i < 3; ++i) {
				short link = triangles_[ via ].linkTo_[ i ];
				if(link != prev && link != -1) {
					if(doLinesIntersectXZ(*b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ], from.localCoor(), to.localCoor())) {
						next = link;
					}
				}
			}
			if(next < 0) {
				return false;
			}
			
			prev = via;
			via = next;
		}
		return true;
	}


	short NavMesh
	::findExit(const BoundingBox& wantedAreaBounds, Point3& out) const {
		out.reset();
		int match = 0;
		/*
		for(int i = 0; i < controlPointCount_; ++i) {
			if(wantedAreaBounds.isTouching(controlPoints_[ i ])) {
				// Is outside on right side??
				out.add(controlPoints_[ i ]);
				++match;
			}
		}
		*/


		for(int i = 0; i < triangleCount_; ++i) {
			out.reset();
			match = 0;
			for(int j = 0; j < 3; ++j) {
				if(wantedAreaBounds.isTouching(controlPoints_[ triangles_[i].controlPoints_[j] ])) {
					// Is outside on right side??
					out.add(controlPoints_[ triangles_[ i ].controlPoints_[j] ]);
					++match;
				}
				if(match >= 2) {
					out.scale(0.999f / (float)match);
					Point3 c;
					wantedAreaBounds.center(c);
					c.scale(0.001);
					out.add(c);
					return i;
				}
			}
		}
		return -1;
	}


}

