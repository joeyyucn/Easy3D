#ifndef _EMATH_H_INCLUDED_
#define _EMATH_H_INCLUDED_
#include "com_types.h"
#include <cstringt.h>

inline void VECTOR4D_COPY(VECTOR4D_PTR src, VECTOR4D_PTR dest)
{
	memcpy(dest, src, sizeof(VECTOR4D));
}

inline void VECTOR4D_ADD(VECTOR4D_PTR va, VECTOR4D_PTR vb, VECTOR4D_PTR dest)
{
	dest->x = va->x + vb->x;
	dest->y = va->y + vb->y;
	dest->z = va->z + vb->z;
	dest->w = va->w + vb->w;
}

void Mat_Init_4x4(MATRIX4X4_PTR mt, float , float, float, float, 
				  float, float, float, float, 
				  float, float, float, float, 
				  float, float, float, float);

void Build_Modal_To_World_MATRIX4X4(VECTOR4D_PTR vpos, MATRIX4X4_PTR mt);
void Mat_VECTOR4D_Mul_MATRIX4X4(VECTOR4D_PTR vect, MATRIX4X4_PTR mt, VECTOR4D_PTR dest);

enum TransformCoordinate
{
	TRANSFORM_LOCAL_ONLY,
	TRANSFORM_TRANS_ONLY,
	TRANSFORM_LOCAL_TO_TRANS,
};

void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mt, TransformCoordinate coord_select);
void Transform_OBJECT4DV1(OBJECT4DV1_PTR object, MATRIX4X4_PTR mt, TransformCoordinate coord_select, bool trans_basis);
void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, TransformCoordinate coord_select = TRANSFORM_LOCAL_TO_TRANS);
void Model_To_World_RENDERLIST(RENDERLIST4DV1_PTR rend_list, VECTOR4D_PTR world_pos, TransformCoordinate coord_select = TRANSFORM_LOCAL_TO_TRANS);
#endif