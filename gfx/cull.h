#pragma once
#include "render_types.h"
#include "camera.h"

#define CULL_OBJECT_X_PLANE	0x1
#define CULL_OBJECT_Y_PLANE	0x2
#define CULL_OBJECT_Z_PLANE	0x4
#define CULL_OBJECT_ALL	( CULL_OBJECT_X_PLANE | CULL_OBJECT_Y_PLANE | CULL_OBJECT_Z_PLANE )

int Cull_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam, int cull_mode );

void Reset_OBJECT4DV1( OBJECT4DV1_PTR obj );

void Remove_Backfaces_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam );