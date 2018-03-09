#pragma once
#include "common_matrix.h"
#include "common_vector.h"
#include "render_types.h"
#include "camera.h"
enum 
{
	TRANSFORM_LOCAL_ONLY, 
	TRANSFORM_TRANS_ONLY,
	TRANSFORM_LOCAL_TO_TRANS,
};

void Build_Modal_To_World_MATRIX4X4(VECTOR4D_PTR vpos, MATRIX4X4_PTR mt);

void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mt, int coord_select);

void Transform_OBJECT4DV1(OBJECT4DV1_PTR object, MATRIX4X4_PTR mt, int coord_select, bool trans_basis);

void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select = TRANSFORM_LOCAL_TO_TRANS);

void Model_To_World_RENDERLIST(RENDERLIST4DV1_PTR rend_list, VECTOR4D_PTR world_pos, int coord_select = TRANSFORM_LOCAL_TO_TRANS);

void World_To_Camera_OBJECT4DV1( CAMERA4DV1_PTR cam, OBJECT4DV1_PTR obj );

void World_To_Camera_RENDERLIST4DV1( CAMERA4DV1_PTR cam, RENDERLIST4DV1_PTR render_list );

void Camera_To_Perspective_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam );