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


#ifndef base_noise_Perlin_hpp
#define base_noise_Perlin_hpp

#include "util/type/util_type.hpp"

namespace se_core {
	class _SeCoreExport Perlin {
	public:
		Perlin();
		Perlin(const unsigned int seed);
		void defaultPermutations();
		void shufflePermutations(const unsigned int seed);

		float noise( float x,
					 const unsigned int frequency,
					 const bool tile );
		float noise( float x, float y,
					 const unsigned int frequencyX, const unsigned int frequencyY,
					 const bool tileX, const bool tileY );
		float noise( float x, float y, float z,
					 const unsigned int frequencyX, const unsigned int frequencyY, const unsigned int frequencyZ,
					 const bool tileX, const bool tileY, const bool tileZ );
		float noise( float x, float y, float z, float w,
					 const unsigned int frequencyX, const unsigned int frequencyY,
					 const unsigned int frequencyZ, const unsigned int frequencyW,
					 const bool tileX, const bool tileY, const bool tileZ, const bool tileW );

	protected:
		static int floor( const float x ) { return ((int)(x) - ((x) < 0 && (x) != (int)(x))); }
		static int tileMod( const int index, const unsigned int frequency );
		static float fade( const float t ) { return t * t * t * (t * (t * 6 - 15) + 10); }
		static float lerp( const float t, const float a, const float b ) { return a + t * (b - a); }
		static int abs( int v) { return (v < 0) ? -v : v; }
		static float abs( float v) { return (v < 0) ? -v : v; }

		static float gradient( const int hash, const float x );
		static float gradient( const int hash, const float x, const float y );
		static float gradient( const int hash, const float x, const float y, const float z );
		static float gradient( const int hash, const float x, const float y, const float z, const float w );

	private:
		unsigned char permutations_[512];
		static const unsigned char defaultPermutations_[256];
	};
}

#endif
