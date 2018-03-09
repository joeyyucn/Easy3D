#pragma once
#include "common.h"
#include <memory>
////////////////////////////////////////////////////////////////////////////////
//	MATRIX
////////////////////////////////////////////////////////////////////////////////
typedef struct MATRIX2X2_TYP
{
	union
	{
		float m[4];
		struct
		{
			float m00, m01;
			float m10, m11;
		};
	};
}MATRIX2X2, *MATRIX2X2_PTR;

typedef struct MATRX3X3_TYP
{
	union
	{
		float M[9];
		struct
		{
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};
	};
}MATRIX3X3, *MATRIX3X3_PTR;

typedef struct MATRIX4X4_TYP
{
	union
	{
		float M[16];
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
	};
}MATRIX4X4, *MATRIX4X4_PTR;

const MATRIX4X4 IMAT_4X4 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
const MATRIX3X3 IMAT_3X3 = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
const MATRIX2X2 IMAT_2X2 = { 1, 0, 0, 1 };

#define MAT_ZERO_2X2(m)	{ memset( (void*)m, 0, sizeof(MATRIX2X2) );}
#define MAT_ZERO_3X3(m)	{ memset( (void*)m, 0, sizeof(MATRIX3X3) );}
#define MAT_ZERO_4X4(m)	{ memset( (void*)m, 0, sizeof(MATRIX4X4) );}

#define MAT_IDENTITY_2X2(m) { memcpy( (void*)m, (void*)&IMAT_2X2, sizeof(MATRIX2X2) ); }
#define MAT_IDENTITY_3X3(m) { memcpy( (void*)m, (void*)&IMAT_3X3, sizeof(MATRIX3X3) ); }
#define MAT_IDENTITY_4X4(m) { memcpy( (void*)m, (void*)&IMAT_4X4, sizeof(MATRIX4X4) ); }

#define MAT_COPY_2X2( src_mat, dest_mat ) { memcpy( (void*)dest_mat, (void*)src_mat, sizeof(MATRIX2X2) ); }
#define MAT_COPY_4X4( src_mat, dest_mat ) { memcpy( (void*)dest_mat, (void*)src_mat, sizeof(MATRIX4X4) ); }
#define MAT_COPY_3X3( src_mat, dest_mat ) { memcpy( (void*)dest_mat, (void*)src_mat, sizeof(MATRIX3X3) ); }

FORCE_INLINE void MAT_TRANSPOSE_3X3( MATRIX3X3_PTR m )
{
	MATRIX3X3 mt;
	mt.m00 = m->m00; mt.m01 = m->m10; mt.m02 = m->m20;
	mt.m10 = m->m01; mt.m11 = m->m11; mt.m12 = m->m21;
	mt.m20 = m->m02; mt.m21 = m->m12; mt.m22 = m->m22;

	memcpy( m, &mt, sizeof(MATRIX3X3) );
}

FORCE_INLINE void MAT_TRANSPOSE_4X4( MATRIX4X4_PTR m )
{
	MATRIX4X4 mt;
	mt.m00 = m->m00; mt.m01 = m->m10; mt.m02 = m->m20; mt.m03 = m->m30; 
	mt.m10 = m->m01; mt.m11 = m->m11; mt.m12 = m->m21; mt.m13 = m->m31;
	mt.m20 = m->m02; mt.m21 = m->m12; mt.m22 = m->m22; mt.m23 = m->m32;
	mt.m30 = m->m03; mt.m31 = m->m13; mt.m32 = m->m23; mt.m33 = m->m33;

	memcpy( m, &mt, sizeof(MATRIX4X4) );
}

FORCE_INLINE void MAT_TRANSPOSE_3X3( MATRIX3X3_PTR m, MATRIX3X3_PTR mt )
{
	mt->m00 = m->m00; mt->m01 = m->m10; mt->m02 = m->m20;
	mt->m10 = m->m01; mt->m11 = m->m11; mt->m12 = m->m21;
	mt->m20 = m->m02; mt->m21 = m->m12; mt->m22 = m->m22;
}

FORCE_INLINE void MAT_TRANSPOSE_4X4( MATRIX4X4_PTR m, MATRIX4X4_PTR mt )
{
	mt->m00 = m->m00; mt->m01 = m->m10; mt->m02 = m->m20; mt->m03 = m->m30; 
	mt->m10 = m->m01; mt->m11 = m->m11; mt->m12 = m->m21; mt->m13 = m->m31;
	mt->m20 = m->m02; mt->m21 = m->m12; mt->m22 = m->m22; mt->m23 = m->m32;
	mt->m30 = m->m03; mt->m31 = m->m13; mt->m32 = m->m23; mt->m33 = m->m33;
}

FORCE_INLINE void MAT_INIT_2X2( MATRIX2X2_PTR ma, float m00, float m01, float m10, float m11 )
{
	ma->m00 = m00; ma->m01 = m01;
	ma->m10 = m10; ma->m11 = m11;
}

FORCE_INLINE void MAT_ADD_2X2( MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR mresult )
{
	mresult->m00 = ma->m00 + mb->m00; mresult->m01 = ma->m01 + mb->m01;
	mresult->m10 = ma->m10 + mb->m10; mresult->m11 = ma->m11 + mb->m11;
}

FORCE_INLINE void MAT_MUL_2X2( MATRIX2X2_PTR ma, MATRIX2X2_PTR mb, MATRIX2X2_PTR mresult )
{
	mresult->m00 = ma->m00 * mb->m00 + ma->m01 * mb->m10;
	mresult->m01 = ma->m00 * mb->m01 + ma->m01 * mb->m11;

	mresult->m10 = ma->m10 * mb->m00 + ma->m11 * mb->m10;
	mresult->m11 = ma->m10 * mb->m01 + ma->m11 * mb->m11;
}

FORCE_INLINE void MAT_INIT_3X3( MATRIX3X3_PTR ma, float m00, float m01, float m02,
							   float m10, float m11, float m12, 
							   float m20, float m21, float m22 )
{
	ma->m00 = m00; ma->m01 = m01; ma->m02 = m02; 
	ma->m10 = m10; ma->m11 = m11; ma->m12 = m12;
	ma->m20 = m20; ma->m21 = m21; ma->m22 = m22;
}

FORCE_INLINE void MAT_ADD_3X3( MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mresult )
{
	mresult->m00 = ma->m00 + mb->m00; mresult->m01 = ma->m01 + mb->m01; mresult->m02 = ma->m02 + mb->m02;
	mresult->m10 = ma->m10 + mb->m10; mresult->m11 = ma->m11 + mb->m11; mresult->m12 = ma->m12 + mb->m12;
	mresult->m20 = ma->m20 + mb->m20; mresult->m21 = ma->m21 + mb->m21; mresult->m22 = ma->m22 + mb->m22;
}

FORCE_INLINE void MAT_MUL_3X3( MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mresult )
{
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < 3; j++ )
		{
			mresult->M[3*i + j] = 0.0f;
			for( int k = 0; k < 3; k++ )
			{
				mresult->M[3*i + j] += ma->M[i*3 + k] * mb->M[k*3 + j];
			}
		}
	}
}

FORCE_INLINE float MAT_DET_2X2( MATRIX2X2_PTR m )
{
	return m->m00 * m->m11 - m->m01 * m->m10;
}

FORCE_INLINE bool MAT_INVERT_2X2( MATRIX2X2_PTR m, MATRIX2X2_PTR mi )
{
	float det = MAT_DET_2X2( m );
	if( det == 0.0f )
	{
		return false;
	}
	det = 1/det;
	mi->m00 = det * m->m11;
	mi->m01 = - (det * m->m01);
	mi->m10 = - (det * m->m10);
	mi->m11 = det * m->m00;

	return true;
}

FORCE_INLINE float MAT_DET_3X3( MATRIX3X3_PTR m )
{
	return m->m00 * ( m->m11 * m->m22 - m->m12 * m->m21 )
		- m->m01 * ( m->m10 * m->m22 - m->m12 * m->m20 )
		+ m->m02 * ( m->m10 * m->m21 - m->m20 * m->m11 );
}

FORCE_INLINE bool MAT_INVERT_3X3( MATRIX3X3_PTR m, MATRIX3X3_PTR mi )
{
	float det = MAT_DET_3X3( m );
	if( det == 0.0f )
	{
		return false;
	}

	det = 1/det;
	mi->m00 = ( m->m11 * m->m22 - m->m21 * m->m12 ) * det;
	mi->m01 = ( m->m02 * m->m21 - m->m01 * m->m22 ) * det;
	mi->m02 = ( m->m01 * m->m12 - m->m02 * m->m11 ) * det;

	mi->m10 = ( m->m12 * m->m20 - m->m10 * m->m22 ) * det;
	mi->m11 = ( m->m00 * m->m22 - m->m02 * m->m20 ) * det;
	mi->m12 = ( m->m10 * m->m02 - m->m00 * m->m12 ) * det;

	mi->m20 = ( m->m10 * m->m21 - m->m20 * m->m11 ) * det;
	mi->m21 = ( m->m20 * m->m01 - m->m00 * m->m21 ) * det;
	mi->m22 = ( m->m00 * m->m11 - m->m10 * m->m01 ) * det;

	return true;
}

FORCE_INLINE float MAT_DET_4X4( MATRIX4X4_PTR m )
{
	return m->m03 * m->m12 * m->m21 * m->m30 - m->m02 * m->m13 * m->m21 * m->m30 -
		m->m03 * m->m11 * m->m22 * m->m30 + m->m01 * m->m13 * m->m22 * m->m30 +
		m->m02 * m->m11 * m->m23 * m->m30 - m->m01 * m->m12 * m->m23 * m->m30 -
		m->m03 * m->m12 * m->m20 * m->m31 + m->m02 * m->m13 * m->m20 * m->m31 +
		m->m03 * m->m10 * m->m22 * m->m31 - m->m00 * m->m13 * m->m22 * m->m31 -
		m->m02 * m->m10 * m->m23 * m->m31 + m->m00 * m->m12 * m->m23 * m->m31 +
		m->m03 * m->m11 * m->m20 * m->m32 - m->m01 * m->m13 * m->m20 * m->m32 -
		m->m03 * m->m10 * m->m21 * m->m32 + m->m00 * m->m13 * m->m21 * m->m32 +
		m->m01 * m->m10 * m->m23 * m->m32 - m->m00 * m->m11 * m->m23 * m->m32 -
		m->m02 * m->m11 * m->m20 * m->m33 + m->m01 * m->m12 * m->m20 * m->m33 +
		m->m02 * m->m10 * m->m21 * m->m33 - m->m00 * m->m12 * m->m21 * m->m33 -
		m->m01 * m->m10 * m->m22 * m->m33 + m->m00 * m->m11 * m->m22 * m->m33;
}

FORCE_INLINE bool MAT_INVERT_4X4( MATRIX4X4_PTR m , MATRIX4X4_PTR mi )
{
	float det = 0.0f;
	mi->M[0] = m->M[5] * m->M[10] * m->M[15] - m->M[5] * m->M[11] * m->M[14] - m->M[9] * m->M[6] * m->M[15]
	+ m->M[9] * m->M[7] * m->M[14] + m->M[13] * m->M[6] * m->M[11] - m->M[13] * m->M[7] * m->M[10];
	mi->M[4] = -m->M[4] * m->M[10] * m->M[15] + m->M[4] * m->M[11] * m->M[14] + m->M[8] * m->M[6] * m->M[15]
	- m->M[8] * m->M[7] * m->M[14] - m->M[12] * m->M[6] * m->M[11] + m->M[12] * m->M[7] * m->M[10];
	mi->M[8] =   m->M[4]*m->M[9]*m->M[15] - m->M[4]*m->M[11]*m->M[13] - m->M[8]*m->M[5]*m->M[15]
	+ m->M[8]*m->M[7]*m->M[13] + m->M[12]*m->M[5]*m->M[11] - m->M[12]*m->M[7]*m->M[9];
	mi->M[12] = -m->M[4]*m->M[9]*m->M[14] + m->M[4]*m->M[10]*m->M[13] + m->M[8]*m->M[5]*m->M[14]
	- m->M[8]*m->M[6]*m->M[13] - m->M[12]*m->M[5]*m->M[10] + m->M[12]*m->M[6]*m->M[9];
	mi->M[1] =  -m->M[1]*m->M[10]*m->M[15] + m->M[1]*m->M[11]*m->M[14] + m->M[9]*m->M[2]*m->M[15]
	- m->M[9]*m->M[3]*m->M[14] - m->M[13]*m->M[2]*m->M[11] + m->M[13]*m->M[3]*m->M[10];
	mi->M[5] =   m->M[0]*m->M[10]*m->M[15] - m->M[0]*m->M[11]*m->M[14] - m->M[8]*m->M[2]*m->M[15]
	+ m->M[8]*m->M[3]*m->M[14] + m->M[12]*m->M[2]*m->M[11] - m->M[12]*m->M[3]*m->M[10];
	mi->M[9] =  -m->M[0]*m->M[9]*m->M[15] + m->M[0]*m->M[11]*m->M[13] + m->M[8]*m->M[1]*m->M[15]
	- m->M[8]*m->M[3]*m->M[13] - m->M[12]*m->M[1]*m->M[11] + m->M[12]*m->M[3]*m->M[9];
	mi->M[13] =  m->M[0]*m->M[9]*m->M[14] - m->M[0]*m->M[10]*m->M[13] - m->M[8]*m->M[1]*m->M[14]
	+ m->M[8]*m->M[2]*m->M[13] + m->M[12]*m->M[1]*m->M[10] - m->M[12]*m->M[2]*m->M[9];
	mi->M[2] =   m->M[1]*m->M[6]*m->M[15] - m->M[1]*m->M[7]*m->M[14] - m->M[5]*m->M[2]*m->M[15]
	+ m->M[5]*m->M[3]*m->M[14] + m->M[13]*m->M[2]*m->M[7] - m->M[13]*m->M[3]*m->M[6];
	mi->M[6] =  -m->M[0]*m->M[6]*m->M[15] + m->M[0]*m->M[7]*m->M[14] + m->M[4]*m->M[2]*m->M[15]
	- m->M[4]*m->M[3]*m->M[14] - m->M[12]*m->M[2]*m->M[7] + m->M[12]*m->M[3]*m->M[6];
	mi->M[10] =  m->M[0]*m->M[5]*m->M[15] - m->M[0]*m->M[7]*m->M[13] - m->M[4]*m->M[1]*m->M[15]
	+ m->M[4]*m->M[3]*m->M[13] + m->M[12]*m->M[1]*m->M[7] - m->M[12]*m->M[3]*m->M[5];
	mi->M[14] = -m->M[0]*m->M[5]*m->M[14] + m->M[0]*m->M[6]*m->M[13] + m->M[4]*m->M[1]*m->M[14]
	- m->M[4]*m->M[2]*m->M[13] - m->M[12]*m->M[1]*m->M[6] + m->M[12]*m->M[2]*m->M[5];
	mi->M[3] =  -m->M[1]*m->M[6]*m->M[11] + m->M[1]*m->M[7]*m->M[10] + m->M[5]*m->M[2]*m->M[11]
	- m->M[5]*m->M[3]*m->M[10] - m->M[9]*m->M[2]*m->M[7] + m->M[9]*m->M[3]*m->M[6];
	mi->M[7] =   m->M[0]*m->M[6]*m->M[11] - m->M[0]*m->M[7]*m->M[10] - m->M[4]*m->M[2]*m->M[11]
	+ m->M[4]*m->M[3]*m->M[10] + m->M[8]*m->M[2]*m->M[7] - m->M[8]*m->M[3]*m->M[6];
	mi->M[11] = -m->M[0]*m->M[5]*m->M[11] + m->M[0]*m->M[7]*m->M[9] + m->M[4]*m->M[1]*m->M[11]
	- m->M[4]*m->M[3]*m->M[9] - m->M[8]*m->M[1]*m->M[7] + m->M[8]*m->M[3]*m->M[5];
	mi->M[15] =  m->M[0]*m->M[5]*m->M[10] - m->M[0]*m->M[6]*m->M[9] - m->M[4]*m->M[1]*m->M[10]
	+ m->M[4]*m->M[2]*m->M[9] + m->M[8]*m->M[1]*m->M[6] - m->M[8]*m->M[2]*m->M[5];

	det = m->M[0]*mi->M[0] + mi->M[1]*mi->M[4] + m->M[2]*mi->M[8] + m->M[3]*mi->M[12];

	if( det == 0.0f )
	{
		return false;
	}
	det = 1.0f/det;

	for ( int i = 0;i < 16; i++ )
	{
		mi->M[i] = mi->M[i] * det;
	}
	return true;
}

FORCE_INLINE void MAT_INIT_4X4( MATRIX4X4_PTR m, float m00, float m01, float m02, float m03, 
							   float m10, float m11, float m12, float m13,
							   float m20, float m21, float m22, float m23,
							   float m30, float m31, float m32, float m33 )
{
	m->m00 = m00; m->m01 = m01; m->m02 = m02; m->m03 = m03;
	m->m10 = m10; m->m11 = m11; m->m12 = m12; m->m13 = m13;
	m->m20 = m20; m->m21 = m21; m->m22 = m22; m->m23 = m23;
	m->m30 = m30; m->m31 = m31; m->m32 = m32; m->m33 = m33;
}

FORCE_INLINE void MAT_ADD_4X4( MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mr )
{
	mr->m00 = ma->m00 + mb->m00;
	mr->m01 = ma->m01 + mb->m01;
	mr->m02 = ma->m02 + mb->m02;
	mr->m03 = ma->m03 + mb->m03;

	mr->m10 = ma->m10 + mb->m10;
	mr->m11 = ma->m10 + mb->m11;
	mr->m12 = ma->m12 + mb->m12;
	mr->m13 = ma->m13 + mb->m13;

	mr->m20 = ma->m20 + mb->m20;
	mr->m21 = ma->m21 + mb->m21;
	mr->m22 = ma->m22 + mb->m22;
	mr->m23 = ma->m23 + mb->m23;

	mr->m30 = ma->m30 + mb->m30;
	mr->m31 = ma->m31 + mb->m31;
	mr->m32 = ma->m32 + mb->m32;
	mr->m33 = ma->m33 + mb->m33;
}

FORCE_INLINE void MAT_MUL_4X4( MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mr )
{
	float value = 0.0f;
	for (int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			value = ma->M[i*4] * mb->M[j];
			value += ma->M[i*4 + 1] * mb->M[4 + j];
			value += ma->M[i*4 + 2] * mb->M[8 + j];
			value += ma->M[i*4 + 3] * mb->M[12 + j];
			mr->M[i * 4 + j] = value;
		}
	}
}