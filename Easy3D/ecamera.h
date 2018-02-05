#ifndef _ECAMERA_H_INCLUDED_
#define _ECAMERA_H_INCLUDED_
#include "com_types.h"

typedef struct CAMERA4DV1_TYP
{
	int state;
	int attr;

	POINT4D pos;

	VECTOR4D dir;
	VECTOR4D u, v, n;
	POINT4D target;

	float view_dist_h;
	float view_dist_v;

	float fov;

	float near_clip_z;
	float far_clip_z;

	PLANE3D rt_clip_plane;
	PLANE3D lt_clip_plane;
	PLANE3D tp_clip_plane;
	PLANE3D bt_clip_plane;

	float viewplane_width;
	float viewplane_height;

	float viewport_width;
	float viewport_height;
	float viewport_center_x;
	float viewport_center_y;

	float aspect_ratio;

	MATRIX4X4 mcam;
	MATRIX4X4 mper;
	MATRIX4X4 mscr;
}
CAMERA4DV1, *CAMERA4DV1_PTR;

void Init_CAMERA4DV1(CAMERA4DV1_PTR camera, int attr, POINT4D_PTR world_pos, VECTOR4D_PTR cam_dir, POINT4D_PTR cam_target, 
					 float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height);
#endif