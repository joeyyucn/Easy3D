#pragma once
#include "common_vector.h"
#include "common_types.h"
#include "common_matrix.h"
#include "render_types.h"
enum
{
	CAMERATYPE_EULER,
	CAMERATYPE_UVN,
};

typedef struct CAMERA4DV1_TYP
{
	int state;
	int attr;

	POINT4D pos;

	VECTOR4D dir;

	// UVN ONLY
	VECTOR4D u, v, n;
	POINT4D target;
	// UVN ONLY END

	float view_dist;
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

void Init_CAMERA4DV1( CAMERA4DV1_PTR camera, int attr, POINT4D_PTR world_pos, VECTOR4D_PTR cam_dir, POINT4D_PTR cam_target,
					 float near_clip_z, float far_clip_z, float fov, float viewport_width, float viewport_height );

enum
{
	CAM_ROT_SEQ_XYZ,
	CAM_ROT_SEQ_YXZ,
	CAM_ROT_SEQ_XZY,
	CAM_ROT_SEQ_YZX,
	CAM_ROT_SEQ_ZYX,
	CAM_ROT_SEQ_ZXY,
};
void Build_CAM4DV1_Matrix_Euler( CAMERA4DV1_PTR cam, int cam_rot_seq );

enum
{
	UVN_TARGET_MODE_SPHERICAL,
	UVN_TARGET_MODE_POINT,
};
void Build_CAM4DV1_Matrix_UVN( CAMERA4DV1_PTR cam, int uvn_mode );

void Build_CAM4DV1_To_Perspective_MATRIX4X4( CAMERA4DV1_PTR cam, MATRIX4X4_PTR mt );