#ifndef _COM_TYPES_H_INCLUDED_
#define _COM_TYPES_H_INCLUDED_

typedef struct vector2d
{
	float x, y;
}
vector2d, *vector2d_ptr;

typedef struct vector3d
{
	float x, y, z;
}
vector3d, *vector3d_ptr;

typedef struct vector4d
{
	float x, y, z, w;
}
vector4d, *vector4d_ptr;

typedef struct point2d
{
	float x, y;
}
point2d, *point2d_ptr;

typedef struct point3d
{
	float x, y, z;
}
point3d, *point3d_ptr;

typedef struct point4d
{
	float x, y, z, w;
}
point4d, *point4d_ptr;

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
	
	point4d_ptr vlist;
	int vindex[3];
} poly4dv1, *poly4dv1_ptr;

typedef struct POLYF4DV1_TYP
{
	int attr;
	int state;
	int color;

	point4d vlist[3];
	point4d tlist[3];

	POLYF4DV1_TYP* pPrev;
	POLYF4DV1_TYP* pNext;
} polyf4dv1, polyf4dv1_ptr;

#define OBJECT4DV1_MAX_VERTICES		256
#define OBJECT4DV1_MAX_POLYS		128

#define OBJECT4DV1_STATE_ACTIVE		0x0001
#define OBJECT4DV1_STATE_VISIBLE	0x0002
typedef struct OBJECT4DV1_TYP
{
	int id;
	char name[64];
	int attr;
	int state;
	float avg_radius;
	float max_radius;

	point4d world_pos;
	vector4d dir;
	vector4d ux, uy, uz;

	int num_vertices;
	point4d vlist_local[OBJECT4DV1_MAX_VERTICES];
	point4d vlist_trans[OBJECT4DV1_MAX_VERTICES];
	
	int num_polys;
	poly4dv1 plist[OBJECT4DV1_MAX_POLYS];

} object4dv1, *object4dv1_ptr;

#define RENDERLIST4DV1_MAX_POLYS	256
typedef struct RENDERLIST4DV1_TYP
{
	int state;
	int attr;

	polyf4dv1_ptr poly_ptrs[RENDERLIST4DV1_MAX_POLYS];
	polyf4dv1_ptr poly_data[RENDERLIST4DV1_MAX_POLYS];
	int num_polys;
} renderlist4dv1, *renderlist4dv1_ptr;

#endif