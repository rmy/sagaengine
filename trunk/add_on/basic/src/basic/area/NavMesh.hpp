#ifndef basic_area_NavMesh_hpp
#define basic_area_NavMesh_hpp

#include "BasicPre.hpp"

namespace se_basic {

	bool doLinesIntersectXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1
						  , se_core::Point2* out);

	bool doLinesIntersectXZ(const se_core::Point3& a0
						  , const se_core::Point3& a1
						  , const se_core::Point3& b0
						  , const se_core::Point3& b1);


	struct Triangle {
		short id_;
		short controlPoints_[3];
		short linkTo_[3];
		short linkType_[3];
		short type_;
	};


	class _SeBasicExport Path {
	public:
		/**
		 * Constructs and initialises a Path object.
		 * @param tringleCount number of triangles in navigation mesh
		 * @param paths path lookup table data. Empty array created if null.
		 */
		Path(int triangleCount, unsigned char* paths = 0)
				: triangleCount_(triangleCount) , paths_(paths) {
			if(!paths_) {
				// 4 values per char so size * size / 4
				paths_ = new unsigned char[ dataSize() ];
			}
		}


		/** Destructor */
		~Path() {}


		/**
		 * Set the neighbour creatures should walk through
		 * to get to a given destination triangle. The
		 * neigbourIndex is an index into the from
		 * triangles neighbours_ array.
		 * @see Triangle class.
		 *
		 * @param from index of triangle to move from
		 * @param to index of triangle to move to
		 * @param neighbourIndex must be 0, 1 or 2
		 */
		void set(short from, short to, int neighbourIndex) {
			int index = (from * triangleCount_ + to) >> 2;
			int shift = ((from * triangleCount_ + to) & 0x3) * 2;

			// Reset existing value
			paths_[ index ] &= ~(0x3 << shift);

			// Set new value
			// assert(neighbourIndex & 0x3 == neighbourIndex)
			paths_[ index ] ^= (neighbourIndex << shift);
		}


		/**
		 * Returns the neighbour index of 1, 2 or 3.
		 * @param from index of triangle to move from
		 * @param to index of triangle to move to
		 */
		int path(short from, short to) const {
			int index = (from * triangleCount_ + to) >> 2;
			int shift = ((from * triangleCount_ + to) & 0x3) * 2;

			int ret = (paths_[ index ] >> shift) & 0x3;
			return ret;
		}


		/**
		 * Returns the size of the char array that holds
		 * the navigation matrix. 
		 */
		int dataSize() {
			return (triangleCount_ * triangleCount_ + 3) >> 2;
		}


		/**
		 * Return a pointer to the data holding
		 * the navigation matrix. You probably only
		 * need this when saving or copying its content.
		 * The data is encoded with 4 values inside
		 * each byte.
		 */
		unsigned char* data() {
			return paths_;
		}

		void lineIntersect(const se_core::Point2& p0
						   , const se_core::Point2& p1
						   , const se_core::Point2& p2
						   , const se_core::Point2& p3
						   , se_core::Point2& out) {
			// this function computes the intersection of the sent lines
			// and returns the intersection point, note that the function assumes
			// the lines intersect. the function can handle vertical as well
			// as horizontal lines. note the function isn't very clever, it simply
			//applies the math, but we don't need speed since this is a
			//pre-processing step

			// b1 and b2 unused
			float a1, c1, // constants of linear equations
				a2, c2,
				det_inv,  // the inverse of the determinant of the coefficie matrix
				m1, m2;    // the slopes of each line

			// compute slopes, note the cludge for infinity, however, this will
			// be close enough

			if ((p1.x_ - p0.x_) != 0)
				m1 = (p1.y_ - p0.y_) / (p1.x_ - p0.x_);
			else
				m1 = (float)1e+10;   // close enough to infinity

			if ((p3.x_ - p2.x_) != 0)
				m2 = (p3.y_ - p2.y_) / (p3.x_ - p2.x_);
			else
				m2 = (float)1e+10;   // close enough to infinity

			// compute constants
			a1 = m1;
			a2 = m2;

			c1 = (p0.y_ - m1 * p0.x_);
			c2 = (p2.y_ - m2 * p2.x_);

			// compute the inverse of the determinate
			det_inv = 1 / (a2 - a1);

			// use Kramers rule to compute xi and yi
			out.x_ = ((c1 - c2) * det_inv);
			//out.y_ = ((a2 * c1 - a1 * c2) * det_inv);
			out.y_ = m1 * ( (c2 - c1) / (m1 - m2) ) + c1;
		} // end Intersect_Lines		
		



	private:
		int triangleCount_;
		unsigned char* paths_;
	};


	class _SeBasicExport NavMesh {
	public:
		NavMesh(short controlPointCount, short triangleCount)
				:  controlPointCount_(controlPointCount)
				 , triangleCount_(triangleCount) {
			controlPoints_ = new se_core::Point3[ controlPointCount_ ];
			triangles_ = new Triangle[ triangleCount_ ];
			paths_ = new Path(triangleCount_);
		}


		NavMesh(const void* data) {
			union {
				const void* v;
				//void* v;
				unsigned char* ch;
				short *sh;
				se_core::Point3* cp;
				Triangle* tri;
			} offset;

			offset.v = data;

			controlPointCount_ = *(offset.sh++);
			triangleCount_ = *(offset.sh++);

			controlPoints_ = offset.cp;
			offset.cp += controlPointCount_;

			triangles_ = offset.tri;
			offset.tri += triangleCount_;

			paths_ = new Path(triangleCount_, offset.ch);
			offset.ch += paths_->dataSize();

			/*
			for(int i = 0; i < triangleCount_; ++i) {
				LogMsg(i << ": "
						<< triangles_[ i ].controlPoints_[0] << ", "
						<< triangles_[ i ].controlPoints_[1] << ", "
						<< triangles_[ i ].controlPoints_[2] << ", "
						);
				for(int j = 0; j < 3; ++j) {
					LogMsg("  Link: " << i << "[" << j << "] = " << triangles_[ i ].linkTo_[ j ]);
				}
			}
			for(int i = 0; i < controlPointCount_; ++i) {
				LogMsg(i << ": " << controlPoints_[ i ].x_ << ", " << controlPoints_[ i ].z_ );
			}
			*/
		}


		bool isInLineOfSight(const se_core::Pos& from, const se_core::Pos& to) const;

		/**
		 * Returns index of neighbour to go via.
		 * @param from index of triangle to move from
		 * @param to index of triangle to move to
		 */
		short path(short from, short to) const {
			int neighbourIndex = paths_->path(from, to);
			short via = triangles_[ from ].linkTo_[ neighbourIndex ];
			return via;
		}


		void center(short tri, se_core::Point3& out) const {
			const se_core::Point3& c1 = controlPoints_[ triangles_[tri].controlPoints_[0] ];
			const se_core::Point3& c2 = controlPoints_[ triangles_[tri].controlPoints_[1] ];
			const se_core::Point3& c3 = controlPoints_[ triangles_[tri].controlPoints_[2] ];

			out.x_ = (c1.x_ + c2.x_ + c3.x_) / 3.0f;
			out.y_ = (c1.y_ + c2.y_ + c3.y_) / 3.0f;
			out.z_ = (c1.z_ + c2.z_ + c3.z_) / 3.0f;
		}


		short type(short triangle) {
			return triangles_[ triangle ].type_;
		}

		void barycentric(const se_core::Point2& q, const se_core::Point3& c1, const se_core::Point3& c2, const se_core::Point3& c3, se_core::Tuple3& out) const {
			coor_t b0 =  (c2.x_ - c1.x_) * (c3.z_ - c1.z_) - (c3.x_ - c1.x_) * (c2.z_ - c1.z_);
			out.x_ = ((c2.x_ - q.x_) * (c3.z_ - q.y_) - (c3.x_ - q.x_) * (c2.z_ - q.y_)) / b0;
			out.y_ = ((c3.x_ - q.x_) * (c1.z_ - q.y_) - (c1.x_ - q.x_) * (c3.z_ - q.y_)) / b0;
			out.z_ = 1 - out.x_ - out.y_;
			//out.z = ((c1.x_ - q.x_) * (c2.z_ - q.y_) - (c2.x_ - q.x_) * (c1.z_ - q.y_)) / b0;
		}


		void barycentric(short tri, const se_core::Point2& q, se_core::Tuple3& out) const {
			const se_core::Point3& c1 = controlPoints_[ triangles_[tri].controlPoints_[0] ];
			const se_core::Point3& c2 = controlPoints_[ triangles_[tri].controlPoints_[1] ];
			const se_core::Point3& c3 = controlPoints_[ triangles_[tri].controlPoints_[2] ];

			coor_t b0 =  (c2.x_ - c1.x_) * (c3.z_ - c1.z_) - (c3.x_ - c1.x_) * (c2.z_ - c1.z_);
			out.x_ = ((c2.x_ - q.x_) * (c3.z_ - q.y_) - (c3.x_ - q.x_) * (c2.z_ - q.y_)) / b0;
			out.y_ = ((c3.x_ - q.x_) * (c1.z_ - q.y_) - (c1.x_ - q.x_) * (c3.z_ - q.y_)) / b0;
			//out.z = ((c1.x_ - q.x_) * (c2.z_ - q.y_) - (c2.x_ - q.x_) * (c1.z_ - q.y_)) / b0;
			out.z_ = 1 - out.x_ - out.y_;
		}

		
		coor_t height(short tri, const se_core::Tuple3& barycentric) const {
			const se_core::Point3& c1 = controlPoints_[ triangles_[tri].controlPoints_[0] ];
			const se_core::Point3& c2 = controlPoints_[ triangles_[tri].controlPoints_[1] ];
			const se_core::Point3& c3 = controlPoints_[ triangles_[tri].controlPoints_[2] ];

			coor_t h = barycentric.x_ * c1.y_
				+ barycentric.y_ * c2.y_
				+ barycentric.z_ * c3.y_;

			return h;
		}


		bool isInsideTriangle(se_core::Tuple3& barycentric) const {
			return (barycentric.x_ >= 0 && barycentric.y_ >= 0 && barycentric.z_ >= 0);
		}
		
		/** Barycentric test */
		bool isInsideTriangle(se_core::Point2& q, se_core::Point3& c1, se_core::Point3& c2, se_core::Point3& c3) const {
			se_core::Tuple3 b;
			barycentric(q, c1, c2, c3, b);
			return isInsideTriangle(b);
		}

		/** Clockwise test */
		bool isInsideTriangle(se_core::Point2& q, se_core::Point3* c) const {
			using namespace se_core;

			static int index1[] = { 1, 2, 0 };
			static int index2[] = { 2, 0, 1 };

			for(int i = 0; i < 3; ++i) {
				Point3& c0 = c[ i ];
				Point3& c1 = c[ index1[ i ] ];
				Point3& c2 = c[ index2[ i ] ];

				Vector2 n(-(c1.z_ - c0.z_), c1.x_ - c0.x_);
				Vector2 F(c2.x_ - c0.x_, c2.z_ - c0.z_);
				Vector2 G(q.x_ - c0.x_, q.y_ - c0.z_);

				if(n.dot(F) * n.dot(G) < 0)
					return false;
			}
			return true;
		}


		/** Find the index of a triangle that the point is above.
		 */
		short find(const se_core::Point2& q) const {
			// TODO: Should organize triangles in a tree or grid structure
			se_core::Tuple3 b;
			for(int i = 0; i < triangleCount_; ++i) {
				barycentric(i, q, b);
				if(isInsideTriangle(b)) {
					return i;
				}
			}
			//LogMsg(q.x_ << ", " << q.y_);
			return -1;
		}


		/** Find the index of a triangle that the point is above.
		 */
		short find(short from, const se_core::Point2& q) const {
			Assert(from >= 0);
			short count = 0;

			se_core::Tuple3 b;

			short tri = from;
			barycentric(tri, q, b);
			while(!isInsideTriangle(b)) {
				if(++count > 20) {
					return find(q);
				}
				short n = 0;
				coor_t smallest = b.x_;

				if(b.y_ < smallest) {
					n = 1;
					smallest = b.y_;
				}
				if(b.z_ < smallest) {
					n = 2;
					smallest = b.z_;
				}

				//LogMsg(tri << ": " << n << " (" << b.x_ << ", " << b.y_ << ", " << b.z_ << ") ");
				for(int c = 0; c < 3; ++c) {
					int i = triangles_[ tri ].controlPoints_[ c ];
					//LogMsg(" " << i << ": " << controlPoints_[ i ].x_ << ", " << controlPoints_[ i ].z_ );
				}

				//LogMsg(" Link: " << triangles_[tri].linkTo_[0] << ", " << triangles_[tri].linkTo_[1] << ", " << triangles_[tri].linkTo_[2]);
				tri = triangles_[tri].linkTo_[n];
				if(tri < 0) {
					return -1;
				}
				barycentric(tri, q, b);
			}
			return tri;
		}

		short findExit(const se_core::BoundingBox& bounds, se_core::Point3& out) const;

	protected:
		short controlPointCount_;
		short triangleCount_;

		se_core::Point3* controlPoints_;
		Triangle* triangles_;
		Path* paths_;
	};

}


#endif
