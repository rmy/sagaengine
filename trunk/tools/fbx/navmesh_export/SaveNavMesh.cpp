#include <fbxsdk.h>
#include "SaveCommon.hpp"
#include "DisplayCommon.hpp"
#include "SaveNavMesh.hpp"
#include <stdio.h>
#include <math.h>

#include <fbxfilesdk_nsuse.h>

//------------ Include in your own app - START ------------------

struct Point2 {
	coor_t x_, y_;

	inline Point2() {}
	inline Point2(coor_t x, coor_t y)
			: x_(x), y_(y) {
	}

	inline void set(float x, float y) {
		x_ = x;
		y_ = y;
	}

	inline coor_t dot(Point2& p) {
		return x_ * p.x_ + y_ * p.y_;
	}
};


struct Point3 {
	Point3()
		: x_(0), y_(0), z_(0) {
	}
	Point3(coor_t x, coor_t y, coor_t z)
		: x_(x), y_(y), z_(z) {
	}

	coor_t x_, y_, z_;

	void reset() {
		x_ = y_ = z_;
	}

	void add(Point3& p) {
		x_ += p.x_;
		y_ += p.y_;
		z_ += p.z_;
	}
};


struct Triangle {
	short id_;
	short controlPoints_[3];
	short linkTo_[3];
	short linkType_[3];
	short type_;
};


struct Wall {
	short left_;
	short right_;
};

struct Exit {
	short triangle_;
	short side_;
};

class Path {
public:
	/**
	 * Constructs and initialises a Path object.
	 * @param tringleCount number of triangles in navigation mesh
	 * @param paths path lookup table data. Empty array created if null.
	 */
	Path(int triangleCount, unsigned char* paths = 0)
		: triangleCount_(triangleCount), paths_(paths) {
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


private:
	int triangleCount_;
	unsigned char* paths_;
};

void snapPoint(Point3& p, float size, float snap) {
	float x = p.x_;
	float z = p.z_;
	if(snap != 0) {
		if(x <= 0.1 || x >= (size - 0.1)) {
			p.z_ = floor(z / snap + .5) * snap;
			if(snap < 0) {
				if(z < size / 2) p.z_ = size / 2 + snap;
				if(z > size / 2) p.z_ = size / 2 - snap;
			}	
			DisplayDouble("Moved z: ", p.z_);
		}
		if(z <= 0.1 || z >= (size - 0.1)) {
			p.x_ = floor(x / snap + .5) * snap;
			if(snap < 0) {
				if(x < 32) p.x_ = 32 + snap;
				if(x > 32) p.x_ = 32 - snap;
				}
			DisplayDouble("Moved x: ", p.x_);
		}
	}
	if(x <= .1) {
		p.x_ = 0;
	}
	if(x >= size - .1) {
		p.x_ = size;
	}
	if(z <= .1) {
		p.z_ = 0;
	}
	if(z >= size - .1) {
		p.z_ = size;
	}
}


struct NavMesh {
protected:
	short controlPointCount_;
	short triangleCount_;
	short exitCount_;

	Point3* controlPoints_;
	Wall* walls_;
	Exit* exits_;
	Triangle* triangles_;
	Path* paths_;

public:
	NavMesh(short controlPointCount, short triangleCount)
			:  controlPointCount_(controlPointCount)
			 , triangleCount_(triangleCount)
			 , exitCount_(0) {
		controlPoints_ = new Point3[ controlPointCount_ ];
		triangles_ = new Triangle[ triangleCount_ ];
		paths_ = new Path(triangleCount_);
		exits_ = new Exit[ triangleCount_ * 3 ];
		walls_ = new Wall[ controlPointCount_ ];
	}


	NavMesh(void* data) {
		union {
			void* v;
			unsigned char* ch;
			short *sh;
			Point3* cp;
			Triangle* tri;
			Wall* wall;
			Exit* exit;
		} offset;

		offset.v = data;

		controlPointCount_ = *(offset.sh++);
		triangleCount_ = *(offset.sh++);
		exitCount_ = *(offset.sh++);

		controlPoints_ = offset.cp;
		offset.cp += controlPointCount_;

		triangles_ = offset.tri;
		offset.tri += triangleCount_;

		walls_ = offset.wall;
		offset.wall += controlPointCount_;

		exits_ = offset.exit;
		offset.exit += exitCount_;

		paths_ = new Path(triangleCount_, offset.ch);
		offset.ch += paths_->dataSize();
	}


	/**
	 * Returns index of neighbour to go via.
	 * @param from index of triangle to move from
	 * @param to index of triangle to move to
	 */
	short path(short from, short to) {
		int neighbourIndex = paths_->path(from, to);
		short via = triangles_[ from ].linkTo_[ neighbourIndex ];
		return via;
	}


	short type(short triangle) {
		return triangles_[ triangle ].type_;
	}


	bool isInsideTriangle(Point2& q, Point3& c0, Point3& c1, Point3& c2) {
		Point2 E(c1.x_ - c0.x_, c1.z_ - c0.z_);
		Point2 F(c2.x_ - c0.x_, c2.z_ - c0.z_);
		Point2 G(q.x_ - c0.x_, q.y_ - c0.z_);

		Point2 n;

		n.set(-E.y_, E.x_);
		if(n.dot(q) * n.dot(G) < 0)
			return false;

		n.set(-F.y_, F.x_);
		if(n.dot(q) * n.dot(E) < 0)
			return false;

		n.set(-G.y_, G.x_);
		if(n.dot(q) * n.dot(G) < 0)
			return false;

		return true;
	}


	/** Find the index of a triangle that the point is above.
	 */
	short find(float x, float z) {
		// TODO: Should organize triangles in a tree or grid structure
		Point2 p(x, z);
		for(int i = 0; i < triangleCount_; ++i) {
			Point3& c0 = controlPoints_[ triangles_[i].controlPoints_[0] ];
			Point3& c1 = controlPoints_[ triangles_[i].controlPoints_[1] ];
			Point3& c2 = controlPoints_[ triangles_[i].controlPoints_[2] ];

			if(isInsideTriangle(p, c0, c1, c2))
				return i;
		}
		return -1;
	}


	void alignTile(float size) {
		Point3 max, min;
		if(controlPointCount_ < 1)
			return;

		min = max = controlPoints_[0];
		for(int i = 1; i < controlPointCount_; ++i) {
			if(min.x_ > controlPoints_[i].x_)
				min.x_ = controlPoints_[i].x_;
			if(min.z_ > controlPoints_[i].z_)
				min.z_ = controlPoints_[i].z_;

			if(max.x_ < controlPoints_[i].x_)
				max.x_ = controlPoints_[i].x_;
			if(max.z_ < controlPoints_[i].z_)
				max.z_ = controlPoints_[i].z_;
		}

		Point3 center;
		center.x_ = (min.x_ + max.x_) / 2;
		center.y_ = 0;
		center.z_ = (min.z_ + max.z_) / 2;

		Point3 align;
		align.y_ = 0;
		align.x_ = align.z_ = size / 2;
		while(align.x_ + center.x_ >= size) {
			align.x_ -= size;
		}
		while(align.z_ + center.z_ >= size) {
			align.z_ -= size;
		}

		while(align.x_ + center.x_ < 0) {
			align.x_ += size;
		}
		while(align.z_ + center.z_ < 0) {
			align.z_ += size;
		}

		for(int i = 0; i < controlPointCount_; ++i) {
			controlPoints_[i].add( align );
		}

		DisplayDouble("        Center.x_           ", center.x_ + align.x_);
		DisplayDouble("        Center.z_           ", center.z_ + align.z_);

	}

};

//------------ Include in your own app - END ------------------

/** Utility class for initialising and saving a navmesh.
 */
class SaveableNavMesh : public NavMesh {
public:
	SaveableNavMesh(short controlPointCount, short triangleCount)
			: NavMesh(controlPointCount, triangleCount) {
		for(short i = 0; i < triangleCount_; ++i) {
			triangles_[ i ].id_ = i;
			triangles_[ i ].type_ = i;
		}
	}


	void setControlPoint(short index, KFbxVector4& v) {
		controlPoints_[index].x_ = CoorT::fromFloat(v[0]);
		controlPoints_[index].y_ = CoorT::fromFloat(v[1]);
		controlPoints_[index].z_ = CoorT::fromFloat(v[2]);
		DisplayInt("CPI: ", index);
		DisplayDouble("   Control point.x :", controlPoints_[index].x_);
		DisplayDouble("   Control point.y :", controlPoints_[index].y_);
		DisplayDouble("   Control point.z :", controlPoints_[index].z_);
	}


	void setPath(short from, short to, int neighbourIndex) {
		paths_->set(from, to, neighbourIndex);
	}


	void setCorner(short triangle, short index, short c) {
		triangles_[ triangle ].controlPoints_[index] = c;
	}


	void setLink(short triangle, int index, short to, short type) {
		triangles_[ triangle ].linkTo_[ index ] = to;
		triangles_[ triangle ].linkType_[ index ] = type;
		DisplayInt(" Link from: ", triangle);
		DisplayInt("      to:   ", to);
		DisplayInt("      index:", index);
	}


	void setType(short triangle, short type) {
		triangles_[ triangle ].type_ = type;
	}


	void calcWalls() {
		for(int i = 0; i < controlPointCount_; ++i) {
			walls_[i].left_ = -1;
			walls_[i].right_ = -1;
		}

		for(int i = 0; i < triangleCount_; ++i) {
			Triangle& tri = triangles_[i];
			for(int j = 0; j < 3; ++j) {
				if(tri.linkTo_[j] < 0) {
					int c1 = tri.controlPoints_[ (j + 1) % 3 ];
					DisplayInt("C1: " , c1);
					int c2 = tri.controlPoints_[ (j + 2) % 3 ];
					DisplayInt("C2: " , c2);
					walls_[ c1 ].right_ = c2;
					walls_[ c2 ].left_ = c1;
				}
			}
		}

		DisplayInt("Control points: ", controlPointCount_);
		for(int i = 0; i < controlPointCount_; ++i) {
			Wall& w = walls_[i];
			DisplayInt(" Wall from: ", i);
			DisplayInt("      left: ", w.left_);
			DisplayInt("      right:", w.right_);
			if((w.left_ < 0 && w.right_ >= 0)
			   || (w.left_ >= 0 && w.right_ < 0)) {
				DisplayString("Error");
			}
		}
	}


	void snapOutside(float size, float snap) {
		DisplayDouble("Size : ", size);
		DisplayDouble("Snap : ", snap);
		for(int i = 0; i < controlPointCount_; ++i) {
			snapPoint(controlPoints_[i], size, snap);
		}
	}

	void setExitCodes(float size) {
		for(int i = 0; i < triangleCount_; ++i) {
			Triangle& tri = triangles_[i];
			for(int j = 0; j < 3; ++j) {
				int c1 = tri.controlPoints_[ (j + 1) % 3 ];
				int c2 = tri.controlPoints_[ (j + 2) % 3 ];

				Point3& p1 = controlPoints_[ c1 ];
				Point3& p2 = controlPoints_[ c2 ];

				enum { NORTH = 1, EAST = 1, WEST = 1, SOUTH = 1 };
				if(p1.z_ <= 0 && p2.z_ <= 0) {
					tri.linkType_[ j ] = NORTH;
				}
				if(p1.x_ >= size && p2.x_ >= size) {
					tri.linkType_[ j ] = EAST;
				}
				if(p1.z_ >= size && p2.z_ >= size ) {
					tri.linkType_[ j ] = SOUTH;
				}
				if(p1.x_ <= 0 && p2.x_ <= 0) {
					tri.linkType_[ j ] = WEST;
				}

				if(tri.linkType_[ j ] == 1) {
					exits_[ exitCount_ ].triangle_ = i;
					exits_[ exitCount_ ].side_ = j;
					++exitCount_;
				}
			}
		}
	}


	void save(const char* nodeName, float size) {
		const char* name = nodeName;
		// Strip away "Model::" prefix from node name
		for(int colonCount = 0; *name != 0 && colonCount < 2; ++name) {
			if(*name == ':') {
				++colonCount;
			}
		}

		// No colons found - revert to node name
		if(*name == 0)
			name = nodeName;


		float w = 0, h = 0;
		// Calc extents
		for(int i = 0; i < controlPointCount_; ++i) {
			/*
			controlPoints_[ i ].x_ += .5;
			controlPoints_[ i ].x_ *= 16;
			controlPoints_[ i ].y_ *= 16;
			controlPoints_[ i ].z_ += .5;
			controlPoints_[ i ].z_ *= 16;
			*/

			if(w < controlPoints_[ i ].x_) {
				w = controlPoints_[ i ].x_;
			}
			if(h < controlPoints_[ i ].z_) {
				h = controlPoints_[ i ].z_;
			}
			DisplayDouble("        Width              ", w);
			DisplayDouble("        Height             ", h);
		}


		KString filename(name);
		filename.Append(".bin");
		FILE* out = fopen(filename.Buffer(), "wb");

		SaveHeader('B', 'A', 2, out);

		DisplayString(" Name: ", name);
		SaveString(name, out);

		DisplayInt("        Control Points     ", controlPointCount_);
		//DisplayInt("        Control Point size ", sizeof(Point3));
		DisplayInt("        Triangles          ", triangleCount_);
		//DisplayInt("        Triangle size      ", sizeof(Triangle));


		DisplayDouble("        Width              ", w);
		DisplayDouble("        Height             ", h);
		if(size == 0) {
			SaveShort(static_cast<short>(w + .99999999999), out);
			SaveShort(static_cast<short>(h + .99999999999), out);
		}
		else {
			SaveShort(static_cast<short>(size), out);
			SaveShort(static_cast<short>(size), out);
		}

		short dataSize = 2 + 2 + 2
			+ sizeof(Point3) * controlPointCount_
			+ sizeof(Wall) * controlPointCount_
			+ sizeof(Triangle) * triangleCount_
			+ sizeof(Exit) * exitCount_
			+ paths_->dataSize();

		SaveShort(dataSize, out);

		SaveShort(controlPointCount_, out);
		SaveShort(triangleCount_, out);
		SaveShort(exitCount_, out);

		SaveVoid(controlPoints_
				, sizeof(Point3) * controlPointCount_
				, out
				);

		SaveVoid(walls_
				, sizeof(Wall) * controlPointCount_
				, out
				);

		SaveVoid(triangles_
				, sizeof(Triangle) * triangleCount_
				, out
				);

		SaveVoid(exits_
				, sizeof(Exit) * exitCount_
				, out
				);

		SaveVoid(paths_->data()
				, paths_->dataSize()
				, out
				);



		//SaveVector(controlPoints[i], out);
		//SaveShort(controlPointIndex[j], out);

		fclose(out);
		DisplayString("Saved: ", filename);
	}
};


bool hasControlPoint(KFbxMesh* mesh, int tri, int cp) {
	for(int i = 0; i < 3; ++i) {
		if(mesh->GetPolygonVertex(tri, i) == cp) {
			return true;
		}
	}
	return false;
}


void SaveNavMesh(KFbxNode* node, float size, float snap) {
	KFbxMesh* mesh = (KFbxMesh*) node->GetNodeAttribute ();

	DisplayString("Mesh Name: ", node->GetName());

	int controlPointsCount = mesh->GetControlPointsCount();
	const int polygonCount = mesh->GetPolygonCount();
	static const int MAX_POLY_COUNT = 256;
	if(polygonCount > MAX_POLY_COUNT) throw "Too many polygons";
	SaveableNavMesh navMesh(controlPointsCount, polygonCount);

	KFbxVector4* controlPoints = mesh->GetControlPoints();

	short neighbours[ MAX_POLY_COUNT ][ 3 ];
	float neighbourDistances[ MAX_POLY_COUNT ][ 3 ];
	KFbxVector4 centers[ MAX_POLY_COUNT ];

	for(int i = 0; i < controlPointsCount; ++i) {
		navMesh.setControlPoint(i, controlPoints[i]);
	}
	if(size != 0)
		navMesh.alignTile(size);
	for(int i = 0; i < polygonCount; ++i) {
		int polygonSize = mesh->GetPolygonSize(i);
		// assert(polygonSize == 3);

		int controlPointIndex[3];

		// Save corners indexes
		DisplayInt("Mesh: ", i);
		for(int j = 0; j < 3; ++j) {
			controlPointIndex[j] = mesh->GetPolygonVertex(i, j);
			navMesh.setCorner(i, j, mesh->GetPolygonVertex(i, j));
			DisplayInt("  Corner: ", mesh->GetPolygonVertex(i, j));
		}

		// Find neighbours
		for(int j = 0; j < 3; ++j) {
			neighbours[ i ][ j ] = -1;
			navMesh.setLink(i, j, -1, -1);
		}
		for(int poly = 0; poly < polygonCount; ++poly) {
			// Dont match with self
			if(i == poly) continue;

			// Link is opposite of corner, due to barymetric calculation
			if(hasControlPoint(mesh, poly, controlPointIndex[0])) {
				if(hasControlPoint(mesh, poly, controlPointIndex[1])) {
					neighbours[i][2] = poly;
					navMesh.setLink(i, 2, poly, 0);
				}
				if(hasControlPoint(mesh, poly, controlPointIndex[2])) {
					neighbours[i][1] = poly;
					navMesh.setLink(i, 1, poly, 0);
				}

			}
			if(hasControlPoint(mesh, poly, controlPointIndex[1])) {
				if(hasControlPoint(mesh, poly, controlPointIndex[2])) {
					neighbours[i][0] = poly;
					navMesh.setLink(i, 0, poly, 0);
				}
			}
		}

		// Calculate center
		for(int j = 0; j < 3; ++j) {
			centers[i] += controlPoints[ controlPointIndex[ j ] ];
		}
		centers[i] /= 3;
	}

	// Calc distance to neighbours
	for(int i = 0; i < polygonCount; ++i) {
		for(int j = 0; j < 3; ++j) {
			KFbxVector4 dist = centers[ i ] - centers[ neighbours[ i ] [ j ] ];
			neighbourDistances[ i ][ j ] = dist.Length();
		}
	}

	// Calculate best path
	char path[ MAX_POLY_COUNT ][ MAX_POLY_COUNT ];
	float visited[ MAX_POLY_COUNT ];
	for(int i = 0; i < polygonCount; ++i) {
		// Reset visited
		for(int poly = 0; poly < polygonCount; ++poly) {
			visited[ poly ] = -1;
		}
		// Distance to self is zero
		visited[ i ] = 0;
		path[ i ] [ i ] = 3;

		for(int freeLeft = polygonCount; freeLeft >= 0; --freeLeft) {
			short closest = i;
			short through = i;
			float closestDist = -1;

			// Find nearest
			for(int j = 0; j < polygonCount; ++j) {
				// Distance to polygon
				float dist = visited[ j ];
				// Check that a distance has been calculated yet
				if(dist >= 0) {
					// Iterate through neighbours
					for(int n = 0; n < 3; ++n) {
						// Get polygon index of neighbour
						int nIndex = neighbours[ j ][ n ];
						// Check that a distance to the neighbour has not yet
						// been calculated
						if(nIndex >= 0 && visited[ nIndex ] < 0) {
							// Calculate the distance to neighbour through
							// polygon with index j
							float nDist = dist + neighbourDistances[ j ][ n ];
							// Is this closer than any other yet uncalculated
							// distance?
							if(closestDist < 0 || closestDist > nDist) {
								// Store away distance, target, and
								// where it goes through
								closestDist = nDist;
								closest = nIndex;
								through = j;
							}
						}
					}
				}
			}
			// Register neighbour to go through
			path[ i ][ closest ] = path[ i ] [ through ];
			visited[ closest ] = closestDist;
			DisplayInt("Path i: ", i);
			DisplayInt("Path c: ", closest);
			if(through == i) {
				// Allows for better compresseion using
				// 2 bits saying which neighbour
				for(short n = 0; n < 3; ++n) {
					if(neighbours[ i ][ n ] == closest) {
						path[ i ][ closest ] = n;
					}
				}
				// A bit simpler
				// path[ i ][ closest ] = closest;
			}
		}
	}
	// Compress path array - 2 bits per neighbour index
	//Path* compressedPath = new Path(polygonCount);
	//unsigned char compressedPath[ polygonCount * polygonCount >> 2 ];
	for(int i = 0; i < polygonCount; ++i) {
		for(int j = 0; j < polygonCount; ++j) {
			//int index = (i * polygonCount + j) >> 2;
			//int shift = ((i * polygonCount + j) & 0x3) * 2;
			//compressedPath[ index ] ^= ((path[ i ][ j ] & 0x3) << shift);
			navMesh.setPath(i, j, path[i][j]);
			//compressedPath->set(i, j, path[i][j]);
		}
	}

	for(int i = 0; i < polygonCount; ++i) {
		for(int j = 0; j < polygonCount; ++j) {
			short p = navMesh.path(i, j);
			if(p == -1) {
				DisplayInt("No path From: ", i);
				DisplayInt("        To: ", j);
			}
		}
	}

	navMesh.calcWalls();
	navMesh.snapOutside(size, snap);
	navMesh.setExitCodes(size);

	// Save compressed path
	navMesh.save(node->GetName(), size);

}





void SaveOgre(KFbxNode* node, float size, float snap) {
	KFbxMesh* mesh = (KFbxMesh*) node->GetNodeAttribute ();
	DisplayString("Save ogre: ", node->GetName());


	KString name(node->GetName());
	name.Append("_navmesh.mesh.xml");
	const char* filename = name.Buffer();
	for(int colonCount = 0; *filename != 0 && colonCount < 2; ++filename) {
		if(*filename == ':') {
			++colonCount;
		}
	}
	if(strlen(filename) == 0) {
		filename = name.Buffer();
	}
	FILE* out = fopen(filename, "w");

	int controlPointCount = mesh->GetControlPointsCount();
	const int polygonCount = mesh->GetPolygonCount();

	fprintf(out, "<mesh>\n");
	KFbxVector4* controlPoints = mesh->GetControlPoints();
	fprintf(out, "\t<sharedgeometry vertexcount=\"%d\">\n", controlPointCount);
	fprintf(out, "\t\t<vertexbuffer positions=\"true\" normals=\"true\">\n");
	for(int i = 0; i < controlPointCount; ++i) {
		KFbxVector4& v = controlPoints[i];
		Point3 p(v[0], v[1], v[2]);
		Point3 a(size / 2, 0, size / 2), s(-size / 2, 0, -size / 2);
		p.add(a);
		snapPoint(p, size, snap);
		p.add(s);
		
		
		fprintf(out, "\t\t\t<vertex>\n");
		fprintf(out, "\t\t\t\t<position x=\"%f\" y=\"%f\" z=\"%f\" />\n", p.x_, p.y_, p.z_);
		fprintf(out, "\t\t\t\t<normal x=\"0\" y=\"1\" z=\"0\" />\n");
		fprintf(out, "\t\t\t</vertex>\n");
	}
	fprintf(out, "\t\t</vertexbuffer>\n");
	fprintf(out, "\t</sharedgeometry>\n");

	fprintf(out, "\t<submeshes>\n");
	fprintf(out, "\t\t<submesh material=\"Basic_wireframe\" usesharedvertices=\"true\" use32bitindexes=\"false\" operation_type=\"triangle_list\">\n");
	fprintf(out, "\t\t\t<faces count=\"%d\">\n", polygonCount);
	for(int i = 0; i < polygonCount; ++i) {
		int v[3];
		for(int j = 0; j < 3; ++j) {
			v[j] = mesh->GetPolygonVertex(i, j);
		}
		fprintf(out, "\t\t\t\t<face v1=\"%d\" v2=\"%d\" v3=\"%d\" />\n", v[0], v[1], v[2]);
	}
	fprintf(out, "\t\t\t</faces>\n");
	fprintf(out, "\t\t</submesh>\n");
	fprintf(out, "\t</submeshes>\n");
	fprintf(out, "</mesh>\n");

	fclose(out);
}

