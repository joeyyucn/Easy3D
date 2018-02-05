#pragma once

#include <math.h>
#include <memory>

#define PI	((float)3.141592654f)
#define PI2	((float)6.283185307f)
#define PI_DIV_2	((float)1.570796327f)
#define PI_DIV_4	((float)0.318309886f)

typedef int FIX16;
typedef int *FIXP16_PTR;

#define FIXP16_SHIFT	16
#define FIXP16_MAG		65536
#define FIXP16_DP_MASK	0x0000ffff;
#define FIXP16_WP_MASK	0xffff0000;
#define FIXP16_ROUND_UP	0x00008000;

#define EPSILON_E4		(1E-4)
#define	EPSILON_E5		(1E-5)
#define EPSILON_E6		(1E-6)

////////////////////////////////////////////////////////////////////////////////
//	VECTOR & POINT
////////////////////////////////////////////////////////////////////////////////
typedef struct VECTOR2D_TYP
{
	union 
	{
		float M[2];
		struct
		{
			float x,y;
		};
	};
} VECTOR2D, *VECTOR2D_PTR, POINT2D, *POINT2D_PTR;

typedef struct VECTOR3D_TYP
{
	union 
	{
		float M[3];
		struct
		{
			float x, y, z;
		};
	};
} VECTOR3D, *VECTOR3D_PTR, POINT3D, *POINT3D_PTR;

typedef struct VECTOR2DI_TYP
{
	int x, y;
}VECTOR2DI, *VECTOR2DI_PTR;

typedef struct VECTOR2DF_PTR
{
	float x, y;
}VECTOR2DF, *VECTOR2DF_PTR;

typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4];
		struct
		{
			float x, y, z, w;
		};
	};
} VECTOR4D, *VECTOR4D_PTR, POINT4D, *POINT4D_PTR;

typedef struct PARAMLINE2D_TYP
{
	POINT2D p0;
	POINT2D p1;
	VECTOR2D v;
}PARAMLINE2D, *PARAMLINE2D_PTR;

typedef struct PARAMLINE3D_TYP
{
	POINT3D p0;
	POINT3D P1;
	VECTOR3D v;
} PARAMLINE3D, *PARAMLINE_PTR;

typedef struct PLANE3D_TYP
{
	POINT3D p;
	VECTOR3D n;
}PLANE3D, *PLANE3D_PTR;

////////////////////////////////////////////////////////////////////////////////
//	MATRIX
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
//	QUAT
////////////////////////////////////////////////////////////////////////////////
typedef struct QUAT_TYP
{
	union
	{
		float M[4];
		struct
		{
			float w, x, y, z;
		};
		struct
		{
			float q0;
			VECTOR3D qv;
		};
	};
} QUAT, *QUAT_PTR;

typedef struct POLAR2D_TYP
{
	float r;
	float theta;
}POLAR2D, *POLAR2D_PTR;

typedef struct CYLINDRICAL3D_TYPE
{
	float r;
	float theta;
	float z;
}CYLINDRICAL3D, *CYLINDRICAL3D_PTR;

typedef struct SPHERICAL3D_TYP
{
	float p;
	float theta;
	float phi;
}SPHERICAL3D, *SPHERICAL3D_PTR;

const MATRIX4X4 IMAT_4X4 = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
const MATRIX3X3 IMAT_3X3 = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
const MATRIX2X2 IMAT_2X2 = { 1, 0, 0, 1 };

#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))

#define SWAP(a, b, t) { t=a; a=b; b=t; }

#define DEG_TO_RAD(ang)	((ang)*PI/180.0f)
#define RAD_TO_DEG(rads)	((rads)*180.0f/PI)

#define RAND_RANGE(x, y)	((x) + (rand()%((y) - (x) + 1)))

#define FORCE_INLINE	__forceinline

FORCE_INLINE void VECTOR2D_ZERO( VECTOR2D_PTR v )
{
	v->x = v->y = 0.0f;
}

FORCE_INLINE void VECTOR3D_ZERO( VECTOR3D_PTR v )
{
	v->x = v->y = v->z = 0.0f;
}

FORCE_INLINE void VECTOR4D_ZERO( VECTOR4D_PTR v)
{
	v->x = v->y = v->z = v->w = 0.0f;
}

FORCE_INLINE void VECTOR2D_INITXY( VECTOR2D_PTR v, float x, float y )
{
	v->x = x;
	v->y = y;
}

FORCE_INLINE void VECTOR3D_INITXYZ( VECTOR3D_PTR v, float x, float y, float z )
{
	v->x = x;
	v->y = y;
	v->z = z;
}

FORCE_INLINE void VECTOR4D_INITXYZ( VECTOR4D_PTR v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = 1.0f;
}

FORCE_INLINE void VECTOR2D_INIT( VECTOR2D_PTR vdst, VECTOR2D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
}

FORCE_INLINE void VECTOR3D_INIT( VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
}

FORCE_INLINE void VECTOR4D_INIT( VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
	vdst->w = vsrc->w;
}

FORCE_INLINE void VECTOR2D_COPY( VECTOR2D_PTR vdst, VECTOR2D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
}

FORCE_INLINE void VECTOR3D_COPY( VECTOR3D_PTR vdst, VECTOR3D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
}

FORCE_INLINE void VECTOR4D_COPY( VECTOR4D_PTR vdst, VECTOR4D_PTR vsrc )
{
	vdst->x = vsrc->x;
	vdst->y = vsrc->y;
	vdst->z = vsrc->z;
	vdst->w = vsrc->w;
}

FORCE_INLINE void POINT2D_INIT( POINT2D_PTR dst, POINT2D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
}

FORCE_INLINE void POINT3D_INIT( POINT3D_PTR dst, POINT3D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

FORCE_INLINE void POINT4D_INIT( POINT4D_PTR dst, POINT4D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
	dst->w = src->w;
}

FORCE_INLINE void POINT2D_COPY( POINT2D_PTR dst, POINT2D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
}

FORCE_INLINE void POINT3D_COPY( POINT3D_PTR dst, POINT3D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
}

FORCE_INLINE void POINT4D_COPY( POINT4D_PTR dst, POINT4D_PTR src )
{
	dst->x = src->x;
	dst->y = src->y;
	dst->z = src->z;
	dst->w = src->w;
}

#define MAT_ZERO_2X2(m)	{ memset( (void*)m, 0, sizeof(MATRIX2X2) );}
#define MAT_ZERO_3X3(m)	{ memset( (void*)m, 0, sizeof(MATRIX3X3) );}
#define MAT_ZERO_4X4(m)	{ memset( (void*)m, 0, sizeof(MATRIX4X4) );}

#define MAT_IDENTITY_2X2(m) { memcpy( (void*)m, (void*)IMAT_2X2, sizeof(MATRIX2X2) ); }
#define MAT_IDENTITY_3X3(m) { memcpy( (void*)m, (void*)IMAT_3X3, sizeof(MATRIX3X3) ); }
#define MAT_IDENTITY_4X4(m) { memcpy( (void*)m, (void*)IMAT_4X4, sizeof(MATRIX4X4) ); }

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

FORCE_INLINE void QUAT_ZERO( QUAT_PTR q )
{
	q->x = q->y = q->z = q->w = 0.0f;
}

FORCE_INLINE void QUAT_INITWXYZ( QUAT_PTR q, float w, float x, float y, float z )
{
	q->x = x;
	q->y = y;
	q->z = z;
	q->w = w;
}

FORCE_INLINE void QUAT_INIT_VECTOR3D( QUAT_PTR q, VECTOR3D_PTR v )
{
	q->w = 0.0f;
	q->x = v->x;
	q->y = v->y;
	q->z = v->z;
}

FORCE_INLINE void QUAT_INIT( QUAT_PTR qdst, QUAT_PTR qsrc )
{
	qdst->w = qsrc->w;
	qdst->x = qsrc->x;
	qdst->y = qsrc->y;
	qdst->z = qsrc->z;
}

#define FIXP16_WP(fp) ((fp) >> FIXP16_SHIFT)
#define FIXP16_DP(fp) ((fp) && FIXP16_DP_MASK)
#define INT_TO_FIXP16(i)	(i << FIXP16_SHIFT)
#define FLOAT_TO_FIXP16(f)	(((float)(f) * (float)FIXP16_MAG+0.5))
#define FIXP16_TO_FLOAT(fp)	(((float)fp)/FIXP16_MAG)