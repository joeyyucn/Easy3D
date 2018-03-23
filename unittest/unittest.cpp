// unittest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <Windows.h>
#include "common.h"
#include "common_vector.h"
#include "camera.h"
#include <stddef.h>
#pragma comment( lib, "basicMaths.lib" )
#pragma comment( lib, "gfx.lib" )
#pragma pack(1)

float det( float* m, int rows )
{
	assert( rows < 4 );
	if( rows == 2 )
	{
		return m[0] * m[3] - m[1] * m[2];
	}

	float fdet = 0.0f;
	float subm[9];
	for ( int i = 0; i < rows; i++ )
	{
		int index = 0;
		for ( int j = 0; j < rows*rows; j++ )
		{
			// copy into temporery buffer
			if( j/rows > 0 && j%rows != i )
			{
				subm[index++] = m[j];
			}
		}
		fdet += powf( -1.0f, (float)i ) * m[i] * det(subm, rows - 1 );
	}
	return fdet;
}

//#pragma pack(1) 
typedef unsigned int uint;
union  UnionStruct
{
	/*
	struct
	{
		uint x : 8;
		uint y : 16;
		uint z;
	};
	struct
	{
		uint c : 1;
		uint d : 3;
		uint e : 4;
	};*/
	
	struct
	{
		uint a;
	};

	struct
	{
		uint x: 1;
		uint y: 1;
	};

	uint b;
};

struct BitField
{
	unsigned char alpha : 1;
	/////
	uint b: 1;
	uint a: 2;
	///
	short beta: 1;
	///
	
	
	
};

class A
{
public:
	uint a: 2;
	uint b: 2;
};
union MyUnion
{
	union MyUnion1
	{
		struct MyStruct
		{
			uint i : 1;
		};
	};

	struct 
	{
		uint a : 8;
	};
};

struct TestOffsetStruct
{
	int i;
	int j;
};
int _tmain(int argc, _TCHAR* argv[])
{
	BitField b;
	b.alpha = 0x1;
	b.b = 0x0;
	b.a = 0x1;
	b.beta = 0x1;

	uint c = b.a;
	int size = sizeof( MyUnion );
	size = GetLargePageMinimum();
	return 0;
}

