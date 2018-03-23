#pragma once
#include "common_vector.h"

// polygon and face state
#define POLY4DV1_STATE_ACTIVE	0x0001
#define POLY4DV1_STATE_CLIPPED	0x0002
#define POLY4DV1_STATE_BACKFACE	0x0004

// polygon and face attribute
#define POLY4DV1_ATTR_2SIDE		0x0001
#define POLY4DV1_ATTR_TRANSPARENT	0x0002
#define POLY4DV1_ATTR_8BITCOLOR	0x0004
#define POLY4DV1_ATTR_RGB16		0x0008
#define POLY4DV1_ATTR_RGB24		0x0010

#define POLY4DV1_ATTR_SHADE_MODE_PURE	0x0020
#define POLY4DV1_ATTR_SHADE_MODE_FLAT	0x0040
#define POLY4DV1_ATTR_SHADE_MODE_GOURAUD	0x0080
#define POLY4DV1_ATTR_SHADE_MODE_PHONG	0x0100


typedef struct POLY4DV1_TYP
{
	int attr;
	int state;
	int color;
	
	POINT4D_PTR vlist;
	int vindex[3];
} 
POLY4DV1, *POLY4DV1_PTR;

typedef struct POLYF4DV1_TYP
{
	int attr;
	int state;
	int color;

	POINT4D vlist[3];
	POINT4D tlist[3];

	POLYF4DV1_TYP* pPrev;
	POLYF4DV1_TYP* pNext;
} 
POLYF4DV1, *POLYF4DV1_PTR;

#define OBJECT4DV1_MAX_VERTICES		256
#define OBJECT4DV1_MAX_POLYS		128

#define OBJECT4DV1_STATE_ACTIVE		0x0001
#define OBJECT4DV1_STATE_VISIBLE	0x0002
#define OBJECT4DV1_STATE_CULLED		0x0004
typedef struct OBJECT4DV1_TYP
{
	int id;
	char name[64];
	int attr;
	int state;
	float avg_radius;
	float max_radius;

	POINT4D world_pos;
	VECTOR4D dir;
	VECTOR4D ux, uy, uz;

	int num_vertices;
	POINT4D vlist_local[OBJECT4DV1_MAX_VERTICES];
	POINT4D vlist_trans[OBJECT4DV1_MAX_VERTICES];
	
	int num_polys;
	POLY4DV1 plist[OBJECT4DV1_MAX_POLYS];

} OBJECT4DV1, *OBJECT4DV1_PTR;

#define RENDERLIST4DV1_MAX_POLYS	256
typedef struct RENDERLIST4DV1_TYP
{
	int state;
	int attr;

	POLYF4DV1_PTR poly_ptrs[RENDERLIST4DV1_MAX_POLYS];
	POLYF4DV1 poly_data[RENDERLIST4DV1_MAX_POLYS];
	int num_polys;
} 
RENDERLIST4DV1, *RENDERLIST4DV1_PTR;

typedef struct RECTV1_TYP
{
	union
	{
		int M[4];
		struct 
		{
			int left, right, top, bottom;
		};
	};
}
RECTV1, *RECTV1_PTR;

void Reset_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list );
void Reset_OBJECT4DV1( OBJECT4DV1_PTR obj );
int Insert_OBJECT4DV1_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, OBJECT4DV1_PTR obj );