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


#include "Perlin.hpp"

namespace se_core {

	const unsigned char Perlin
	::defaultPermutations_[256] = {
		151,160,137,91,90,15,131, 13,201,95,96,53,194,233,7,
		225,140,36,103,30,69,142,8,99,37,240,21,10,23,190,6,
		148,247,120,234,75, 0,26,197, 62,94,252,219,203,117,
		35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,
		171,168, 68,175,74,165,71,134,139,48,27,166, 77,146,
		158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161, 1,216,80,
		73,209,76,132,187,208,89,18,169,200,196,135,130,116,
		188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123, 5,202,38,147,118,126,255,82,85,212,207,
		206, 59,227, 47,16,58,17,182,189,28,42, 223,183,170,
		213,119,248,152,2,44,154,163,70,221,153,101,155,167,
		43,172,9,129,22,39,253,19,98,108,110,79,113,224,232,
		178,185, 112,104,218,246,97,228, 251,34,242,193,238,
		210,144,12,191,179,162,241,81,51,145,235,249,14,239,
		107, 49,192,214, 31,181,199,106,157,184, 84,204,176,
		115,121,50,45,127, 4,150,254,138,236,205,93,222,114,
		67, 29,24, 72,243,141,128,195,78, 66,215,61,156,180
	};


	Perlin
	::Perlin() {
		defaultPermutations();
	}


	Perlin
	::Perlin(const unsigned int seed) {
		shufflePermutations(seed);
	}


	void Perlin
	::defaultPermutations() {
		for(short i = 0; i < 256; ++i) {
			permutations_[i] = permutations_[i + 256] = defaultPermutations_[i];
		}
	}


	void Perlin
	::shufflePermutations(const unsigned int seed) {
		const unsigned int modseed = (1402024253 * seed + 586950981) >> 24;		// 0 ÷ 255

		for(short i = 0; i < 256; ++i ) {
			permutations_[i] = permutations_[i + 256] = defaultPermutations_[(modseed + i) & 255];
		}
	}

	float Perlin
	::gradient( const int hash, const float x ) {
		return (hash & 1) ? x : -x;
	}


	float Perlin
	::gradient( const int hash, const float x, const float y) {
		static const float gradients[8][2] = {
			{  1.0f,  1.0f }, // 0
			{  1.0f, -1.0f }, // 1
			{ -1.0f,  1.0f }, // 2
			{ -1.0f, -1.0f }, // 3
			{  1.0f,  0.0f }, // 4
			{ -1.0f,  0.0f }, // 5
			{  0.0f,  1.0f }, // 6
			{  0.0f, -1.0f }  // 7
		};
		int h = hash & 7;
		return x * gradients[h][0] + y * gradients[h][1];
	}


	float Perlin
	::gradient( const int hash, const float x, const float y, const float z) {
		static const float gradients[16][3] = {
			{  1.0f,  1.0f,  0.0f }, // 0
			{ -1.0f,  1.0f,  0.0f }, // 1
			{  1.0f, -1.0f,  0.0f }, // 2
			{ -1.0f, -1.0f,  0.0f }, // 3

			{  1.0f,  0.0f,  1.0f }, // 4
			{ -1.0f,  0.0f,  1.0f }, // 5
			{  1.0f,  0.0f, -1.0f }, // 6
			{ -1.0f,  0.0f, -1.0f }, // 7

			{  0.0f,  1.0f,  1.0f }, // 8
			{  0.0f, -1.0f,  1.0f }, // 9
			{  0.0f,  1.0f, -1.0f }, // 10
			{  0.0f, -1.0f, -1.0f }, // 11

			// padding from 12 to 16 cases, for explanation
			// see Perlin's document on Improved Perlin Noise implementation
			{  1.0f,  1.0f,  0.0f }, // 12
			{  0.0f, -1.0f,  1.0f }, // 13
			{ -1.0f,  1.0f,  0.0f }, // 14
			{  0.0f, -1.0f, -1.0f }, // 15
		};
		int h = hash & 15;
		return x * gradients[h][0] + y * gradients[h][1] + z * gradients[h][2];
	}


	float Perlin
	::gradient( const int hash, const float x, const float y, const float z, const float w) {
		static const float gradients[32][4] = {
			{ -1.0f, -1.0f, -1.0f,  0.0 }, // 0
			{ -1.0f, -1.0f,  1.0f,  0.0 }, // 1
			{ -1.0f,  1.0f, -1.0f,  0.0 }, // 2
			{ -1.0f,  1.0f,  1.0f,  0.0 }, // 3

			{  1.0f, -1.0f, -1.0f,  0.0 }, // 4
			{  1.0f, -1.0f,  1.0f,  0.0 }, // 5
			{  1.0f,  1.0f, -1.0f,  0.0 }, // 6
			{  1.0f,  1.0f,  1.0f,  0.0 }, // 7


			{ -1.0f, -1.0f,  0.0f, -1.0 }, // 8
			{ -1.0f,  1.0f,  0.0f, -1.0 }, // 9
			{  1.0f, -1.0f,  0.0f, -1.0 }, // 10
			{  1.0f,  1.0f,  0.0f, -1.0 }, // 11

			{ -1.0f, -1.0f,  0.0f,  1.0 }, // 12
			{ -1.0f,  1.0f,  0.0f,  1.0 }, // 13
			{  1.0f, -1.0f,  0.0f,  1.0 }, // 14
			{  1.0f,  1.0f,  0.0f,  1.0 }, // 15

			{ -1.0f,  0.0f, -1.0f, -1.0 }, // 16
			{  1.0f,  0.0f, -1.0f, -1.0 }, // 17
			{ -1.0f,  0.0f, -1.0f,  1.0 }, // 18
			{  1.0f,  0.0f, -1.0f,  1.0 }, // 19

			{ -1.0f,  0.0f, -1.0f,  1.0 }, // 20
			{  1.0f,  0.0f, -1.0f,  1.0 }, // 21
			{ -1.0f,  0.0f,  1.0f,  1.0 }, // 22
			{  1.0f,  0.0f,  1.0f,  1.0 }, // 23

			{  0.0f, -1.0f, -1.0f, -1.0 }, // 24
			{  0.0f, -1.0f, -1.0f,  1.0 }, // 25
			{  0.0f, -1.0f,  1.0f, -1.0 }, // 26
			{  0.0f, -1.0f,  1.0f,  1.0 }, // 27

			{  0.0f,  1.0f, -1.0f, -1.0 }, // 28
			{  0.0f,  1.0f, -1.0f,  1.0 }, // 29
			{  0.0f,  1.0f,  1.0f, -1.0 }, // 30
			{  0.0f,  1.0f,  1.0f,  1.0 }, // 31
		};
		int h = hash & 31;
		return x * gradients[h][0] + y * gradients[h][1] + z * gradients[h][2] + w * gradients[h][3];
	}


	int Perlin
	::tileMod( const int index, const unsigned int frequency ) {
		return (index < 0) ? abs(index) * (frequency - 1) % frequency : index % frequency;
	}


	float Perlin
	::noise( float x, const unsigned int frequency, const bool tile ) {
		int X = floor( x );
		x -= X;
		X &= 255;

		int X1 = X + 1;
		if( tile ) X1 = tileMod( X1, frequency );

		return lerp( fade( x ), gradient( permutations_[ X  ], x),
					 gradient( permutations_[ X1 ], x - 1 ));
	}


	float Perlin
	::noise( float x, float y,
			 const unsigned int frequencyX, const unsigned int frequencyY,
			 const bool tileX, const bool tileY ) {
		int X = floor( x );
		int Y = floor( y );

		// fractional parts ( from 0.0 to 1.0 )
		x -= X;
		y -= Y;

		const float a = fade( x );
		const float b = fade( y );

		X &= 255;
		Y &= 255;

		int X1 = X + 1;	if( tileX ) X1 = tileMod( X1, frequencyX );
		int Y1 = Y + 1;	if( tileY ) Y1 = tileMod( Y1, frequencyY );

		int AA = permutations_[ X  ] + Y;		// left		bottom
		int BA = permutations_[ X1 ] + Y;		// right	bottom
		int AB = permutations_[ X  ] + Y1;	// left		top
		int BB = permutations_[ X1 ] + Y1;	// right	top

		const float x_1 = x - 1.0f;
		const float y_1 = y - 1.0f;

		float gAA = gradient( permutations_[ AA ], x, y);
		float gBA = gradient( permutations_[ BA ], x_1, y);
		float gAB = gradient( permutations_[ AB ], x, y_1);
		float gBB = gradient( permutations_[ BB ], x_1, y_1);

		float r = lerp( b, lerp( a, gAA, gBA), lerp( a, gAB, gBB));
		return r;
	}


	float Perlin
	::noise(float x, float y, float z,
			const unsigned int frequencyX, const unsigned int frequencyY, const unsigned int frequencyZ,
			const bool tileX, const bool tileY, const bool tileZ ) {
		int X = floor( x );
		int Y = floor( y );
		int Z = floor( z );

		// fractional parts ( from 0.0 to 1.0 )
		x -= X;
		y -= Y;
		z -= Z;

		const float a = fade( x );
		const float b = fade( y );
		const float c = fade( z );

		X &= 255;
		Y &= 255;
		Z &= 255;

		int X1 = X + 1;	if( tileX ) X1 = tileMod( X1, frequencyX );
		int Y1 = Y + 1;	if( tileY ) Y1 = tileMod( Y1, frequencyY );
		int Z1 = Z + 1;	if( tileZ ) Z1 = tileMod( Z1, frequencyZ );

		//--------------------------
		// indices for 2 noise quads
		//--------------------------

		int AA = permutations_[ X  ] + Y;		// left		bottom
		int BA = permutations_[ X1 ] + Y;		// right	bottom
		int AB = permutations_[ X  ] + Y1;	// left		top
		int BB = permutations_[ X1 ] + Y1;	// right	top

		// front-side quad indices
		int AAA = permutations_[ AA ] + Z;
		int BAA = permutations_[ BA ] + Z;
		int ABA = permutations_[ AB ] + Z;
		int BBA = permutations_[ BB ] + Z;

		// back-side quad indices
		int AAB = permutations_[ AA ] + Z1;
		int BAB = permutations_[ BA ] + Z1;
		int ABB = permutations_[ AB ] + Z1;
		int BBB = permutations_[ BB ] + Z1;

		//-----------------------
		// lerp between the quads
		//-----------------------

		const float x_1 = x - 1.0f;
		const float y_1 = y - 1.0f;
		const float z_1 = z - 1.0f;

		return lerp( c, lerp( b, lerp( a, gradient( permutations_[ AAA ], x  , y  , z   ),
									   gradient( permutations_[ BAA ], x_1, y  , z   )),
							  lerp( a, gradient( permutations_[ ABA ], x  , y_1, z   ),
									gradient( permutations_[ BBA ], x_1, y_1, z   ))),
					 lerp( b, lerp( a, gradient( permutations_[ AAB ], x  , y  , z_1 ),
									gradient( permutations_[ BAB ], x_1, y  , z_1 )),
						   lerp( a, gradient( permutations_[ ABB ], x  , y_1, z_1 ),
								 gradient( permutations_[ BBB ], x_1, y_1, z_1 ))));
	}


	float Perlin
	::noise(float x, float y, float z, float w,
			const unsigned int frequencyX, const unsigned int frequencyY,
			const unsigned int frequencyZ, const unsigned int frequencyW,
			const bool tileX, const bool tileY, const bool tileZ, const bool tileW )
	{
		int X = floor( x );
		int Y = floor( y );
		int Z = floor( z );
		int W = floor( w );

		x -= X;
		y -= Y;
		z -= Z;
		w -= W;

		const float a = fade( x );
		const float b = fade( y );
		const float c = fade( z );
		const float d = fade( w );

		X &= 255;
		Y &= 255;
		Z &= 255;
		W &= 255;

		int X1 = X + 1;	if( tileX ) X1 = tileMod( X1, frequencyX );
		int Y1 = Y + 1;	if( tileY ) Y1 = tileMod( Y1, frequencyY );
		int Z1 = Z + 1;	if( tileZ ) Z1 = tileMod( Z1, frequencyZ );
		int W1 = W + 1;	if( tileW ) W1 = tileMod( W1, frequencyW );

		//----------------------------
		// indices for 2 noise volumes
		//----------------------------

		int AA = permutations_[ X  ] + Y;		// left		bottom
		int BA = permutations_[ X1 ] + Y;		// right	bottom

		int AB = permutations_[ X  ] + Y1;	// left		top
		int BB = permutations_[ X1 ] + Y1;	// right	top

		int AAA = permutations_[ AA ] + Z;	// left		bottom	front
		int BAA = permutations_[ BA ] + Z;	// right	bottom	front
		int ABA = permutations_[ AB ] + Z;	// left		top		front
		int BBA = permutations_[ BB ] + Z;	// right	top		front

		int AAB = permutations_[ AA ] + Z1;	// left		bottom	back
		int BAB = permutations_[ BA ] + Z1;	// right	bottom	back
		int ABB = permutations_[ AB ] + Z1;	// left		top		back
		int BBB = permutations_[ BB ] + Z1;	// right	top		back

		int AAAA = permutations_[ AAA ] + W;	// left		bottom	front	volume1
		int BAAA = permutations_[ BAA ] + W;	// right	bottom	front	volume1
		int ABAA = permutations_[ ABA ] + W;	// left		top		front	volume1
		int BBAA = permutations_[ BBA ] + W;	// right	top		front	volume1
		int AABA = permutations_[ AAB ] + W;	// left		bottom	back	volume1
		int BABA = permutations_[ BAB ] + W;	// right	bottom	back	volume1
		int ABBA = permutations_[ ABB ] + W;	// left		top		back	volume1
		int BBBA = permutations_[ BBB ] + W;	// right	top		back	volume1

		int AAAB = permutations_[ AAA ] + W1;	// left		bottom	front	volume2
		int BAAB = permutations_[ BAA ] + W1;	// right	bottom	front	volume2
		int ABAB = permutations_[ ABA ] + W1;	// left		top		front	volume2
		int BBAB = permutations_[ BBA ] + W1;	// right	top		front	volume2
		int AABB = permutations_[ AAB ] + W1;	// left		bottom	back	volume2
		int BABB = permutations_[ BAB ] + W1;	// right	bottom	back	volume2
		int ABBB = permutations_[ ABB ] + W1;	// left		top		back	volume2
		int BBBB = permutations_[ BBB ] + W1;	// right	top		back	volume2

		//-------------------------
		// lerp between the volumes
		//-------------------------

		const float x_1 = x - 1.0f;
		const float y_1 = y - 1.0f;
		const float z_1 = z - 1.0f;
		const float w_1 = w - 1.0f;

		return lerp( d, lerp( c, lerp( b, lerp( a,  gradient( permutations_[ AAAA ], x  , y  , z  , w   ),
												gradient( permutations_[ BAAA ], x_1, y  , z  , w   )),
									   lerp( a,	gradient( permutations_[ ABAA ], x  , y_1, z  , w   ),
											 gradient( permutations_[ BBAA ], x_1, y_1, z  , w   ))),
							  lerp( b, lerp( a,	gradient( permutations_[ AABA ], x  , y  , z_1, w   ),
											 gradient( permutations_[ BABA ], x_1, y  , z_1, w   )),
									lerp( a,	gradient( permutations_[ ABBA ], x  , y_1, z_1, w   ),
										  gradient( permutations_[ BBBA ], x_1, y_1, z_1, w   )))),
					 lerp( c, lerp( b, lerp( a,	gradient( permutations_[ AAAB ], x  , y  , z  , w_1 ),
											 gradient( permutations_[ BAAB ], x_1, y  , z  , w_1 )),
									lerp( a,	gradient( permutations_[ ABAB ], x  , y_1, z  , w_1 ),
										  gradient( permutations_[ BBAB ], x_1, y_1, z  , w_1 ))),
						   lerp( b, lerp( a,	gradient( permutations_[ AABB ], x  , y  , z_1, w_1 ),
										  gradient( permutations_[ BABB ], x_1, y  , z_1, w_1 )),
								 lerp( a,	gradient( permutations_[ ABBB ], x  , y_1, z_1, w_1 ),
									   gradient( permutations_[ BBBB ], x_1, y_1, z_1, w_1 )))));
	}


	float Perlin
	::noise( const Vector3& v, const unsigned int freq, bool tile) {
		return noise(CoorT::toFloat(v.x_),
					 CoorT::toFloat(v.y_),
					 CoorT::toFloat(v.z_),
					 freq, freq, freq,
					 tile, tile, tile);
	}


}
