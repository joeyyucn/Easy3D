#pragma once

#include <memory>
#include "common.h"
#include "common_vector.h"

typedef struct PARAMLINE2D_TYP
{
	POINT2D p0;
	POINT2D p1;
	VECTOR2D v;
}PARAMLINE2D, *PARAMLINE2D_PTR;

typedef struct PARAMLINE3D_TYP
{
	POINT3D p0;
	POINT3D p1;
	VECTOR3D v;
} PARAMLINE3D, *PARAMLINE3D_PTR;

typedef struct PLANE3D_TYP
{
	POINT3D p;
	VECTOR3D n;
}PLANE3D, *PLANE3D_PTR;

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


FORCE_INLINE void INIT_PARAM_LINE2D( POINT2D_PTR pinit, POINT2D_PTR pterm, PARAMLINE2D_PTR p )
{
	p->p0.x = pinit->x;
	p->p0.y = pinit->y;

	p->p1.x = pterm->x;
	p->p1.y = pterm->y;

	p->v.x = pterm->x - pinit->x;
	p->v.y = pterm->y - pterm->y;
}

FORCE_INLINE void Compute_ParamLine_2D( PARAMLINE2D_PTR p, float t, POINT2D_PTR pt )
{
	pt->x = p->p0.x + p->v.x * t;
	pt->y = p->p0.y + p->v.y * t;
}

enum
{
	PARAM_LINE_NO_INTERSECT,
	PARAM_LINE_INTERSECT_IN_SEGMENT,
	PARAM_LINE_INTERSECT_OUT_SEGMENT,
	PARAM_LINE_INTERSECT_EVERYWHERE,
};
FORCE_INLINE int Intersect_Param_Lines2D( PARAMLINE2D_PTR p1, PARAMLINE2D_PTR p2, float* t1, float* t2 )
{
	float detp1p2 = ( p1->v.x * p2->v.y - p1->v.y * p2->v.x );
	if( detp1p2 < EPSILON_E5 )
	{
		return PARAM_LINE_NO_INTERSECT;
	}

	*t1 = ( p2->v.x * ( p1->p0.y - p2->p0.y ) - p2->v.y * ( p1->p0.x - p2->p0.x ) ) / detp1p2;
	*t2 = ( p1->v.x * ( p1->p0.y - p2->p0.y ) - p1->v.y * ( p1->p0.x - p2->p0.x ) ) / detp1p2;

	if( *t1 >= 0 && *t1 <= 1 && *t2 >= 0 && *t2 <= 1 )
	{
		return PARAM_LINE_INTERSECT_IN_SEGMENT;
	}

	return PARAM_LINE_INTERSECT_OUT_SEGMENT;
}

FORCE_INLINE int Intersect_Param_Lines2D( PARAMLINE2D_PTR p1, PARAMLINE2D_PTR p2, POINT2D_PTR pt )
{
	float detp1p2 = ( p1->v.x * p2->v.y - p1->v.y * p2->v.x );
	if( detp1p2 < EPSILON_E5 )
	{
		return PARAM_LINE_NO_INTERSECT;
	}

	float t1 = ( p2->v.x * ( p1->p0.y - p2->p0.y ) - p2->v.y * ( p1->p0.x - p2->p0.x ) ) / detp1p2;
	float t2 = ( p1->v.x * ( p1->p0.y - p2->p0.y ) - p1->v.y * ( p1->p0.x - p2->p0.x ) ) / detp1p2;

	pt->x = p1->p0.x + p1->v.x * t1;
	pt->y = p1->p0.y + p1->v.y * t2;
	if( t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1 )
	{
		return PARAM_LINE_INTERSECT_IN_SEGMENT;
	}

	return PARAM_LINE_INTERSECT_OUT_SEGMENT;
}

FORCE_INLINE void INIT_PARAM_LINE3D( POINT3D_PTR pinit, POINT3D_PTR pterm, PARAMLINE3D_PTR p )
{
	p->p0.x = pinit->x;
	p->p0.y = pinit->y;
	p->p0.z = pinit->z;

	p->p1.x = pterm->x;
	p->p1.y = pterm->y;
	p->p1.z = pterm->z;

	p->v.x = pterm->x - pinit->x;
	p->v.y = pterm->y - pinit->y;
	p->v.z = pterm->z - pinit->z;
}

FORCE_INLINE void Compute_Param_Line3D( PARAMLINE3D_PTR p, float t, POINT3D_PTR pt )
{
	pt->x = p->p0.x + t * p->v.x;
	pt->y = p->p0.y + t * p->v.y;
	pt->z = p->p0.z + t * p->v.z;
}

FORCE_INLINE void PLANE3D_INIT( PLANE3D_PTR p, POINT3D_PTR p0, VECTOR3D_PTR normal, int normalize )
{
	p->p.x = p0->x;
	p->p.y = p0->y;
	p->p.z = p0->z;

	p->n.x = normal->x;
	p->n.y = normal->y;
	p->n.z = normal->z;

	if( normalize )
	{
		VECTOR3D_NORMALIZE( &p->n , &p->n );
	}
}

// calculate if a point is in a plane, positive\negative space
// return:
// 0: if the point is in plane
// >0: if the point is in the positive space
// <0: if the point is in the negative space
// basiclly this is a dot product operation and the value would be 
// depend on the angle to normal vector
FORCE_INLINE float Compute_Point_In_Plane3D( PLANE3D_PTR p, POINT3D_PTR pt )
{
	return p->n.x * ( pt->x - p->p.x ) + p->n.y * ( pt->y - p->p.y ) + p->n.z * ( pt->z - p->p.z );
}

FORCE_INLINE int Intersect_Param_Line3D_Plane3D( PLANE3D_PTR p, PARAMLINE3D_PTR l, float* t )
{
	float nv = VECTOR3D_DOT( &p->n, &l->v );
	if( nv == 0 )
	{
		if( Compute_Point_In_Plane3D( p, &l->p0 ) == 0.0f )
		{
			return PARAM_LINE_INTERSECT_EVERYWHERE;
		}
		else
		{
			return PARAM_LINE_NO_INTERSECT;
		}
	}

	*t = (p->p.x - l->p0.x) * p->n.x + ( p->p.y - l->p0.y ) * p->n.y + ( p->p.z - l->p0.z ) * p->n.z / nv;
	if( *t >= 0 && *t <= 1 )
	{
		return PARAM_LINE_INTERSECT_IN_SEGMENT;
	}
	else
	{
		return PARAM_LINE_INTERSECT_OUT_SEGMENT;
	}
	
}