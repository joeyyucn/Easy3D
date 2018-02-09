#pragma once
#include "common.h"
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

FORCE_INLINE void VECTOR2D_ADD( VECTOR2D_PTR va, VECTOR2D_PTR vb, VECTOR2D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
}

FORCE_INLINE void VECTOR3D_ADD( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
}

FORCE_INLINE void VECTOR4D_ADD( VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vsum )
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = va->w + vb->w;
}

FORCE_INLINE void VECTOR2D_SUB( VECTOR2D_PTR va, VECTOR2D_PTR vb, VECTOR2D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
}

FORCE_INLINE void VECTOR3D_SUB( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
}

FORCE_INLINE void VECTOR4D_SUB( VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR vdiff )
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
	vdiff->w = va->w - vb->w;
}

FORCE_INLINE void VECTOR2D_SCALE( float k, VECTOR2D_PTR va, VECTOR2D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
}

FORCE_INLINE void VECTOR3D_SCALE( float k, VECTOR3D_PTR va, VECTOR3D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
	vscaled->z = va->z * k;
}

FORCE_INLINE void VECTOR4D_SCALE( float k, VECTOR4D_PTR va, VECTOR4D_PTR vscaled )
{
	vscaled->x = va->x * k;
	vscaled->y = va->y * k;
	vscaled->z = va->z * k;
	vscaled->w = va->w * k;
}

FORCE_INLINE float VECTOR2D_DOT( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	return va->x * vb->x + va->y * vb->y;
}

FORCE_INLINE float VECTOR3D_DOT( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}

FORCE_INLINE float VECTOR4D_DOT( VECTOR4D_PTR va, VECTOR4D_PTR vb )
{
	// IGNORE W
	return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}

FORCE_INLINE void VECTOR3D_CROSS( VECTOR3D_PTR va, VECTOR3D_PTR vb, VECTOR3D_PTR cross )
{
	cross->x = va->y * vb->z - va->z * vb->y;
	cross->y = va->z * vb->x - va->x * vb->z;
	cross->z = va->x * vb->y - va->y * vb->x;
}

FORCE_INLINE float VECTOR2D_LENGTH( VECTOR2D_PTR v )
{
	return sqrt( v->x * v->x + v->y * v->y );
}

FORCE_INLINE float VECTOR3D_LENGTH( VECTOR3D_PTR v )
{
	return sqrt( v->x * v->x + v->y * v->y + v->z * v->z );
}

FORCE_INLINE void VECTOR2D_NORMALIZE( VECTOR2D_PTR v )
{
	float len = VECTOR2D_LENGTH( v );
	float safeLenth = I_fsel( len, len, 1.0f );
	float ilength = 1/len;
	v->x *= ilength;
	v->y *= ilength;
}

FORCE_INLINE void VECTOR3D_NORMALIZE( VECTOR3D_PTR v )
{
	float len = VECTOR3D_LENGTH( v );
	float safeLenth = I_fsel( len, len, 1.0f );
	float ilength = 1.0f / safeLenth;
	v->x *= ilength;
	v->y *= ilength;
	v->z *= ilength;
}

FORCE_INLINE void VECTOR4D_NORMALIZE( VECTOR4D_PTR v )
{
	float len = sqrt( v->x * v->x + v->y * v->y + v->z * v->z  );
	float safeLen = I_fsel( len, len, 1.0f );
	float ilength = 1.0f/safeLen;
	v->x *= ilength;
	v->y *= ilength;
	v->z *= ilength;
	v->w *= ilength;
}

FORCE_INLINE void VECTOR2D_NORMALIZE( VECTOR2D_PTR v, VECTOR2D_PTR vout )
{
	float len = VECTOR2D_LENGTH( v );
	float safeLenth = I_fsel( len, len, 1.0f );
	float ilength = 1/len;
	vout->x = v->x * ilength;
	vout->y = v->y * ilength;
}

FORCE_INLINE void VECTOR3D_NORMALIZE( VECTOR3D_PTR v, VECTOR3D_PTR vout )
{
	float len = VECTOR3D_LENGTH( v );
	float safeLenth = I_fsel( len, len, 1.0f );
	float ilength = 1.0f / safeLenth;
	vout->x = v->x * ilength;
	vout->y = v->y * ilength;
	vout->z = v->z * ilength;
}

FORCE_INLINE void VECTOR4D_NORMALIZE( VECTOR4D_PTR v, VECTOR4D_PTR vout )
{
	float len = sqrt( v->x * v->x + v->y * v->y + v->z * v->z  );
	float safeLen = I_fsel( len, len, 1.0f );
	float ilength = 1.0f/safeLen;
	vout->x = v->x * ilength;
	vout->y = v->y * ilength;
	vout->z = v->z * ilength;
	vout->w = v->w * ilength;
}

FORCE_INLINE void VECTOR2D_BUILD( VECTOR2D_PTR v, VECTOR2D_PTR term, VECTOR2D_PTR result )
{
	result->x = term->x - v->x;
	result->y = term->y - v->y;
}

FORCE_INLINE void VECTOR3D_BUILD( VECTOR3D_PTR v, VECTOR3D_PTR term, VECTOR3D_PTR result )
{
	result->x = term->x - v->x;
	result->y = term->y - v->y;
	result->z = term->z - v->z;
}

FORCE_INLINE float VECTOR2D_COSTH( VECTOR2D_PTR va, VECTOR2D_PTR vb )
{
	return VECTOR2D_DOT( va, vb ) / ( VECTOR2D_LENGTH( va ) * VECTOR2D_LENGTH( vb ) );
}

FORCE_INLINE float VECTOR3D_COSTH( VECTOR3D_PTR va, VECTOR3D_PTR vb )
{
	return VECTOR3D_DOT( va, vb ) / ( VECTOR3D_LENGTH( va ) * VECTOR3D_LENGTH( vb ) );
}
