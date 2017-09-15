#include "emath.h"
#include <assert.h>

void Mat_VECTOR4D_Mul_MATRIX4X4(VECTOR4D_PTR vect, MATRIX4X4_PTR mt, VECTOR4D_PTR dest)
{
	for(int i = 0; i < 4; i++)
	{
		 dest->m[i] = 0;
		 for(int j  = 0; j < 4; j++)
		 {
			 dest->m[i] += mt->m[j][i] * vect->m[j];
		 }
	}
}

void Mat_Init_4x4(MATRIX4X4_PTR mt, 
				  float m00, float m01, float m02, float m03, 
				  float m10, float m11, float m12, float m13, 
				  float m20, float m21, float m22, float m23, 
				  float m30, float m31, float m32, float m33)
{
	mt->m[0][0] = m00;mt->m[0][1] = m01;mt->m[0][2] = m02;mt->m[0][3] = m03;
	mt->m[1][0] = m10;mt->m[1][1] = m11;mt->m[1][2] = m12;mt->m[1][3] = m13;
	mt->m[2][0] = m20;mt->m[2][1] = m21;mt->m[2][2] = m22;mt->m[2][3] = m23;
	mt->m[3][0] = m30;mt->m[3][1] = m31;mt->m[3][2] = m32;mt->m[3][3] = m33;
}

void Build_Modal_To_World_MATRIX4X4(VECTOR4D_PTR vpos, MATRIX4X4_PTR mt)
{
	Mat_Init_4x4(mt, 1, 0, 0, 0, 
		0, 1, 0, 0,
		0, 0, 1, 0,
		vpos->x, vpos->y, vpos->z, 1);
}

void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mt, TransformCoordinate coord_select)
{
	VECTOR4D result;
	for( int i = 0; i < render_list->num_polys; i++)
	{
		POLYF4DV1_PTR current_poly = render_list->poly_ptrs[i];
		if(current_poly == NULL || 
			current_poly->state & POLY4DV1_STATE_CLIPPED ||
			current_poly->state & POLY4DV1_STATE_BACKFACE)
			continue;

		switch(coord_select)
		{
		case TRANSFORM_LOCAL_ONLY:
			for(int j = 0; j < 3; j++)
			{
				Mat_VECTOR4D_Mul_MATRIX4X4(&current_poly->vlist[j], mt, &result);
				VECTOR4D_COPY(&result, &current_poly->vlist[j]);
			}
			break;
		case TRANSFORM_TRANS_ONLY:
			for(int j = 0; j < 3; j++)
			{
				Mat_VECTOR4D_Mul_MATRIX4X4(&current_poly->tlist[j], mt, &result);
				VECTOR4D_COPY(&result, &current_poly->tlist[j]);
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			for(int j = 0; j < 3; j++)
			{
				Mat_VECTOR4D_Mul_MATRIX4X4(&current_poly->vlist[j], mt, &current_poly->tlist[j]);
			}
			break;
		default:
			break;
		}
	}
}

void Transform_OBJECT4DV1(OBJECT4DV1_PTR object, MATRIX4X4_PTR mt, TransformCoordinate coord_select, bool trans_basis)
{
	VECTOR4D result;

	switch(coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
		for(int i = 0; i < object->num_vertices; i++)
		{
			Mat_VECTOR4D_Mul_MATRIX4X4(&object->vlist_local[i], mt, &result);
			VECTOR4D_COPY(&result, &object->vlist_local[i]);
		}
		break;
	case TRANSFORM_TRANS_ONLY:
		for(int i = 0; i < object->num_vertices; i++)
		{
			Mat_VECTOR4D_Mul_MATRIX4X4(&object->vlist_trans[i], mt, &result);
			VECTOR4D_COPY(&result, &object->vlist_trans[i]);
		}
		break;
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int i = 0; i < object->num_vertices; i++)
		{
			Mat_VECTOR4D_Mul_MATRIX4X4(&object->vlist_local[i], mt, &object->vlist_trans[i]);
		}
		break;
	default:
		break;
	}

	if(trans_basis)
	{
		VECTOR4D result;
		Mat_VECTOR4D_Mul_MATRIX4X4(&object->ux, mt, &result);
		VECTOR4D_COPY(&result, &object->ux);
		Mat_VECTOR4D_Mul_MATRIX4X4(&object->uy, mt, &result);
		VECTOR4D_COPY(&result, &object->uy);;
		Mat_VECTOR4D_Mul_MATRIX4X4(&object->uz, mt, &result);
		VECTOR4D_COPY(&result, &object->uz);;
	}
}

void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, TransformCoordinate coord_select)
{
	assert(coord_select == TRANSFORM_LOCAL_ONLY || coord_select == TRANSFORM_LOCAL_TO_TRANS);
	if(coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int i = 0; i < obj->num_vertices; i++)
		{
			VECTOR4D_ADD(&obj->vlist_local[i], &obj->world_pos, &obj->vlist_trans[i]);
		}
	}
	else
	{
		for(int i = 0; i < obj->num_vertices; i++)
		{
			VECTOR4D_ADD(&obj->vlist_local[i], &obj->world_pos, &obj->vlist_local[i]);
		}
	}
}

void Model_To_World_RENDERLIST(RENDERLIST4DV1_PTR rend_list, VECTOR4D_PTR world_pos, TransformCoordinate coord_select)
{
	assert(coord_select == TRANSFORM_LOCAL_ONLY || coord_select == TRANSFORM_LOCAL_TO_TRANS);
	if(coord_select == TRANSFORM_LOCAL_TO_TRANS)
	{
		for(int i = 0; i < rend_list->num_polys; i++)
		{
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[i];
			if(curr_poly == NULL || curr_poly->state & POLY4DV1_STATE_BACKFACE 
				|| curr_poly->state & POLY4DV1_STATE_CLIPPED)
			{
				continue;
			}

			for(int j = 0; i < 3; j++)
			{
				VECTOR4D_ADD(&curr_poly->vlist[j], world_pos, &curr_poly->tlist[j]);
			}
		}
	}
	else
	{
		for(int i = 0; i < rend_list->num_polys; i++)
		{
			POLYF4DV1_PTR curr_poly = rend_list->poly_ptrs[i];
			if(curr_poly == NULL || curr_poly->state & POLY4DV1_STATE_BACKFACE 
				|| curr_poly->state & POLY4DV1_STATE_CLIPPED)
			{
				continue;
			}

			for(int j = 0; i < 3; j++)
			{
				VECTOR4D_ADD(&curr_poly->vlist[j], world_pos, &curr_poly->vlist[j]);
			}
		}
	}
}