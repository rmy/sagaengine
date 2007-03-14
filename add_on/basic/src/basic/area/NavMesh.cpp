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
		Point2 tmp;
		return (tmp.lineIntersect(a0, a1, b0, b1) != -1);
		/*
		float s1 = left( b0, b1, a0) * left( b0, b1, a1);
		float s2 = left( a0, a1, b0) * left( a0, a1, b1);
		return (s1 <= 0 && s2 < 0);

		//return doLinesIntersectXZ(a0, a1, b0, b1, 0);
		*/
	}


	bool willLineAIntersectBXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1) {
		Point2 tmp;
		return (tmp.willAIntersectB(a0, a1, b0, b1));
		/*
		float s1 = left( b0, b1, a0) * left( b0, b1, a1);
		float s2 = left( a0, a1, b0) * left( a0, a1, b1);
		return (s1 <= 0 && s2 < 0);

		//return doLinesIntersectXZ(a0, a1, b0, b1, 0);
		*/
	}

	/*
	bool doLinesIntersectXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1
						  , se_core::Point2* out) {
		// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/

		coor_t numeratorA = ((b1.x_ - b0.x_) * (a0.z_ - b0.z_)
			 - (b1.z_ - b0.z_) * (a0.x_ - b0.x_));
		coor_t numeratorB = ((a1.x_ - a0.x_) * (a0.z_ - b0.z_)
			 - (a1.z_ - a0.z_) * (a0.x_ - b0.x_));

		coor_t divisor = ((b1.z_ - b0.z_) * (a1.x_ - a0.x_)
			 - (b1.x_ - b0.x_) * (a1.z_ - a0.z_));

		if(numeratorA == 0 && numeratorB == 0 && divisor == 0) {
			AssertWarning(!doLinesIntersectXZ(a0, a1, b0, b1), divisor);
			return false;
		}

		if(divisor == 0) {
			AssertWarning(!doLinesIntersectXZ(a0, a1, b0, b1), divisor);
			return false;
		}

		coor_t ua = numeratorA / divisor;
		coor_t ub = numeratorB / divisor;

		bool res = (ua >= 0 && ua < 1 && ub >= 0 && ub < 1);
		if(res && out) {
			out->x_ = a0.x_ + ua * (a1.x_ - a0.x_);
			out->y_ = a0.z_ + ua * (a1.z_ - a0.z_);
		}
		//if(ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
		AssertWarning(res == doLinesIntersectXZ(a0, a1, b0, b1), ua << " - " << ub);
		//LogMsg(ua << ", " << ub << ": " << a0.x_ + ua * (a1.x_ - a0.x_) << ", " << a0.z_ + ua * (a1.z_ - a0.z_) << ": " << (doLinesIntersectXZ(a0, a1, b0, b1) ? "true" : "false") );
		return res;
	}
	*/


	bool NavMesh
	::isInLineOfSight(const se_core::Pos& from, const se_core::Pos& to) const {
		return isInLineOfSight(from.localCoor(), from.index(), to.localCoor(), to.index());
	}


	bool NavMesh
	::isInLineOfSight(const se_core::Point3& from, short fromIndex, const se_core::Point3& to, short toIndex) const {
		static const short corners[][2] = {
			{ 1, 2 },
			{ 2, 0 },
			{ 0, 1 }
		};

		short via = fromIndex;
		short prev = -1;
		while(via != toIndex) {
			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			short next = -1;
			for(int i = 0; i < 3; ++i) {
				short link = triangles_[ via ].linkTo_[ i ];
				if(link != prev && link != -1) {
					if(willLineAIntersectBXZ(from, to, *b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ])) {
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


	bool NavMesh
	::doesTouchVoid(const se_core::Point3& p, short pIndex, const coor_t radius) const {
		static const int STACK_SIZE = 10;
		static const short corners[][2] = {
			{ 1, 2 },
			{ 2, 0 },
			{ 0, 1 }
		};

		Point3 nearest;
		const coor_double_t radiusSq = CoorT::pow2(radius);

		struct {
			short index_;
			short edge_;
		} stack[STACK_SIZE];

		int sp = 0;
		stack[sp].index_ = pIndex;
		stack[sp].edge_ = 0;
		short prev = -1;

		while(sp > 0 || stack[sp].edge_ <= 2) {
			const short via = stack[sp].index_;
			const short i = stack[sp].edge_;
			++stack[sp].edge_;

			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			nearest.nearestPoint(*b[corners[ i ][ 0 ]], *b[corners[ i ][ 1 ]], p);
			if(nearest.distanceSquared(p) < radiusSq) {
				const short link = triangles_[ via ].linkTo_[ i ];
				if(link == -1) {
					return true;
				}
				if(link == prev) {
					continue;
				}
				prev = stack[sp].index_;
				++sp;
				Assert(sp < STACK_SIZE);
				stack[sp].index_ = link;
				stack[sp].edge_ = 0;
			}
			while(stack[sp].edge_ > 2 && sp > 0) {
				--sp;
				prev = (sp > 0) ? stack[sp - 1].index_ : -1;
			}
		}
		return false;
	}


	void NavMesh
	::farthestLineOfSightXZ(const se_core::Pos& from, const se_core::Point3& to, short toIndex, Point2& dest) const {
		static const short corners[][2] = {
			{ 1, 2 },
			{ 2, 0 },
			{ 0, 1 }
		};

		short via = from.index();
		short prev = -2;
		dest.set(from.localCoor().x_, from.localCoor().z_);
		while(via != toIndex) {
			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			short next = -1;
			for(int i = 0; i < 3; ++i) {
				short link = triangles_[ via ].linkTo_[ i ];
				if(link != prev) {
					if(dest.lineIntersect(*b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ], from.localCoor(), to) != -1) {
					//if(doLinesIntersectXZ(*b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ], from.localCoor(), to, &dest)) {
						next = link;
						//LogMsg("Found link: " << via << ", " << next << ", " << toIndex << " (" << dest.x_ << ", " << dest.y_ << ")");
					}
				}
			}
			if(next < 0) {
				//LogMsg("Changed coor: " << from.localCoor() << to << " (" << dest.x_ << ", " << dest.y_ << ")");
				return;
			}

			prev = via;
			via = next;
		}
		dest.set(to.x_, to.z_);
	}


	bray_t NavMesh
	::slideAngle(const se_core::Point3& from, short fromIndex, const se_core::Point3& to) const {
		static const short corners[][2] = {
			{ 1, 2 },
			{ 2, 0 },
			{ 0, 1 }
		};
		bray_t currentYaw = from.yawTowards(to);

		short via = fromIndex;
		short prev = -2;
		while(via != -1) {
			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			short next = -1;
			for(int i = 0; i < 3; ++i) {
				short link = triangles_[ via ].linkTo_[ i ];
				if(link != prev) {
					if(doLinesIntersectXZ(*b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ], from, to)) {
						if(link < 0) {
							// NOTE: Found edge - exiting!!!
							bray_t slideYaw = b[ corners[ i ][ 0 ] ]->yawTowards(*b[ corners[ i ][ 1 ] ]);
							if(BrayT::abs(BrayT::sub(currentYaw, slideYaw)) > BrayT::DEG90) {
								slideYaw = BrayT::invert(slideYaw);
								slideYaw += BRAY_RES / 4;
							}
							else {
								slideYaw -= BRAY_RES / 4;
							}
							if(BrayT::abs(BrayT::sub(currentYaw, slideYaw)) >= 96 * BRAY_RES) {
								return currentYaw;
							}
							return slideYaw;
						}
						next = link;
					}
				}
			}

			prev = via;
			via = next;
		}
		return currentYaw;
	}


	bray_t NavMesh
	::wallAngle(const se_core::Point3& from, short fromIndex, const se_core::Point3& to) const {
		static const short corners[][2] = {
			{ 1, 2 },
			{ 2, 0 },
			{ 0, 1 }
		};
		Point2 tmp;


		bray_t currentYaw = from.yawTowards(to);
		short via = fromIndex;
		Assert(via != -1);
		short prev = -2;
		while(via != -1) {
			Point3* b[] = {
				&controlPoints_[ triangles_[ via ].controlPoints_[ 0 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 1 ] ],
				&controlPoints_[ triangles_[ via ].controlPoints_[ 2 ] ]
			};
			short next = -2;
			short count = 0;
			for(int i = 0; i < 3; ++i) {
				short link = triangles_[ via ].linkTo_[ i ];
				if(link != prev) {
					if(tmp.willAIntersectB(from, to, *b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ])) {
						if(link < 0 && next != -1) {
							int wall = triangles_[ via ].controlPoints_[ corners[ i ][ 0 ] ];
							scale_t alpha = tmp.lineIntersect(*b[ corners[ i ][ 0 ] ], *b[ corners[ i ][ 1 ] ], from, to);
							bray_t wallYaw;
							if(alpha < .25f) {
								Point3& middle = controlPoints_[ wall ];
								Point3& leftP = controlPoints_[ walls_[ wall ].left_ ];
								Point3& right = controlPoints_[ walls_[ wall ].right_ ];
								if(left(leftP, right, middle) > 0) {
									wallYaw = leftP.yawTowards(right);
								}
								else {
									wallYaw = middle.yawTowards(right);
								}
							}
							else if(alpha > .75f) {
								wall = walls_[ wall ].right_;
								Point3& middle = controlPoints_[ wall ];
								Point3& leftP = controlPoints_[ walls_[ wall ].left_ ];
								Point3& right = controlPoints_[ walls_[ wall ].right_ ];
								wallYaw = leftP.yawTowards(right);
								if(left(leftP, right, middle) > 0) {
									wallYaw = leftP.yawTowards(right);
								}
								else {
									wallYaw = leftP.yawTowards(middle);
								}
							}
							else {
								Assert(alpha > 0);
								Point3& left = controlPoints_[ wall ];
								Point3& right = controlPoints_[ walls_[ wall ].right_ ];
								wallYaw = left.yawTowards(right);
							}

							if(BrayT::abs(BrayT::sub(currentYaw, wallYaw)) > BrayT::DEG90) {
								wallYaw = BrayT::invert(wallYaw);
							}
							return wallYaw;
						}
						next = link;
					}
				}
			}

			prev = via;
			via = next;
		}
		LogFatal("Couldn't find wall yaw");
		return currentYaw;
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
					c.scale(0.001f);
					out.add(c);
					return i;
				}
			}
		}
		return -1;
	}


	coor_double_t NavMesh
	::findNearest(const Point3& p, Point3& out) const {
		Point3 tmp;
		coor_double_t minDistSq = 0;

		for(int i = 0; i < triangleCount_; ++i) {
			for(int j = 0; j < 3; ++j) {
				tmp.nearestPoint(controlPoints_[ triangles_[ i ].controlPoints_[j] ]
						, controlPoints_[ triangles_[ i ].controlPoints_[(j + 1) % 3] ], p);
				if(minDistSq == 0 || tmp.distanceSquared(p) < minDistSq) {
					minDistSq = tmp.distanceSquared(p);
					out.set(tmp);
				}
			}
		}
		return minDistSq;
	}

}

