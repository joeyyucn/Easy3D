#include <cstringt.h>
#include "ecamera.h"
#include "emath.h"


void Init_CAMERA4DV1(CAMERA4DV1_PTR camera, int attr, POINT4D_PTR world_pos, VECTOR4D_PTR cam_dir, 
					 POINT4D_PTR cam_target, float near_clip_z, float far_clip_z, float fov, 
					 float viewport_width, float viewport_height)
{
	memset(camera, 0, sizeof(CAMERA4DV1));
	camera->attr = attr;
	VECTOR4D_COPY(world_pos, &camera->pos);
	VECTOR4D_COPY(cam_dir, &camera->dir);
}