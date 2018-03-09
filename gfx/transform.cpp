#include "transform.h"
#include "common_matrix.h"
void Build_Modal_To_World_MATRIX4X4(VECTOR4D_PTR vpos, MATRIX4X4_PTR mt)
{
	MAT_INIT_4X4( mt, 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		vpos->x, vpos->y, vpos->z, 1 );
}


void Transform_RENDERLIST4DV1(RENDERLIST4DV1_PTR render_list, MATRIX4X4_PTR mt, int coord_select)
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
				VECTOR4D_Mul_MATRIX4X4(&current_poly->vlist[j], mt, &result);
				VECTOR4D_COPY(&result, &current_poly->vlist[j]);
			}
			break;
		case TRANSFORM_TRANS_ONLY:
			for(int j = 0; j < 3; j++)
			{
				VECTOR4D_Mul_MATRIX4X4(&current_poly->tlist[j], mt, &result);
				VECTOR4D_COPY(&result, &current_poly->tlist[j]);
			}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
			for(int j = 0; j < 3; j++)
			{
				VECTOR4D_Mul_MATRIX4X4(&current_poly->vlist[j], mt, &current_poly->tlist[j]);
			}
			break;
		default:
			break;
		}
	}
}

void Transform_OBJECT4DV1(OBJECT4DV1_PTR object, MATRIX4X4_PTR mt, int coord_select, bool trans_basis)
{
	VECTOR4D result;

	switch(coord_select)
	{
	case TRANSFORM_LOCAL_ONLY:
		for(int i = 0; i < object->num_vertices; i++)
		{
			VECTOR4D_Mul_MATRIX4X4(&object->vlist_local[i], mt, &result);
			VECTOR4D_COPY(&result, &object->vlist_local[i]);
		}
		break;
	case TRANSFORM_TRANS_ONLY:
		for(int i = 0; i < object->num_vertices; i++)
		{
			VECTOR4D_Mul_MATRIX4X4(&object->vlist_trans[i], mt, &result);
			VECTOR4D_COPY(&result, &object->vlist_trans[i]);
		}
		break;
	case TRANSFORM_LOCAL_TO_TRANS:
		for(int i = 0; i < object->num_vertices; i++)
		{
			VECTOR4D_Mul_MATRIX4X4(&object->vlist_local[i], mt, &object->vlist_trans[i]);
		}
		break;
	default:
		break;
	}

	if(trans_basis)
	{
		VECTOR4D result;
		VECTOR4D_Mul_MATRIX4X4(&object->ux, mt, &result);
		VECTOR4D_COPY(&result, &object->ux);
		VECTOR4D_Mul_MATRIX4X4(&object->uy, mt, &result);
		VECTOR4D_COPY(&result, &object->uy);;
		VECTOR4D_Mul_MATRIX4X4(&object->uz, mt, &result);
		VECTOR4D_COPY(&result, &object->uz);;
	}
}

void Model_To_World_OBJECT4DV1(OBJECT4DV1_PTR obj, int coord_select)
{
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

void Model_To_World_RENDERLIST(RENDERLIST4DV1_PTR rend_list, VECTOR4D_PTR world_pos, int coord_select)
{
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

void World_To_Camera_OBJECT4DV1( CAMERA4DV1_PTR cam, OBJECT4DV1_PTR obj )
{
	for (int i =0; i < obj->num_vertices; i++ )
	{
		VECTOR4D result;
		VECTOR4D_Mul_MATRIX4X4( &obj->vlist_trans[i], &cam->mcam, &result );
		VECTOR4D_COPY( &obj->vlist_trans[i], &result );
	}
}

void World_To_Camera_RENDERLIST4DV1( CAMERA4DV1_PTR cam, RENDERLIST4DV1_PTR render_list )
{
	for ( int i = 0; i <render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];
		if ( poly == NULL || !( poly->state & POLY4DV1_STATE_ACTIVE )
			|| ( poly->state & POLY4DV1_STATE_CLIPPED ) || ( poly->state & POLY4DV1_STATE_BACKFACE ) )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			POINT4D result;
			VECTOR4D_Mul_MATRIX4X4( &poly->tlist[i], &cam->mcam, &result );
			VECTOR4D_COPY( &poly->tlist[i], &result );
		}
	}
}

void Camera_To_Perspective_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam )
{
	for ( int i = 0; i < obj->num_vertices; i++ )
	{
		obj->vlist_trans[i].x = obj->vlist_trans[i].x * cam->view_dist / obj->vlist_trans[i].z;
		obj->vlist_trans[i].y = obj->vlist_trans[i].y * cam->view_dist / obj->vlist_trans[i].z;
	}
}