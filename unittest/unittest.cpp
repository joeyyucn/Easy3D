// unittest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>
#include <math.h>
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
		fdet += powf( -1, i ) * m[i] * det(subm, rows - 1 );
	}
	return fdet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//float m[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f };
	float m[] = { 5, 3, 3, 4, 2, 3, 5, 7, 4};
	printf("%f \n", det(m, 3));
	return 0;
}

