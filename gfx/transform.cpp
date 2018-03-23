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
		VECTOR4D result = { 0, 0, 0, 0 };
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
			VECTOR4D_ZERO( &result );
			VECTOR4D_Mul_MATRIX4X4( &poly->tlist[j], &cam->mcam, &result );
			VECTOR4D_COPY( &poly->tlist[j], &result );
		}
	}
}

void Camera_To_Perspective_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam )
{
	for ( int i = 0; i < obj->num_vertices; i++ )
	{
		obj->vlist_trans[i].x = obj->vlist_trans[i].x * cam->view_dist / obj->vlist_trans[i].z;
		obj->vlist_trans[i].y = obj->vlist_trans[i].y * cam->view_dist * cam->aspect_ratio / ( obj->vlist_trans[i].z );
	}
}

void Convert_From_Homogeneous4D_OBJECT4DV1( OBJECT4DV1_PTR obj )
{
	for ( int i = 0; i < obj->num_vertices; i++ )
	{
		float divw = 1.0f / obj->vlist_trans[i].w;
		obj->vlist_trans[i].x *= divw;
		obj->vlist_trans[i].y *= divw;
		obj->vlist_trans[i].z *= divw;
		obj->vlist_trans[i].w = 1.0f;
	}
}

void Camera_To_Perspective_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, CAMERA4DV1_PTR cam )
{
	for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];
		
		if ( poly == NULL || !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_CLIPPED ||
			poly->state & POLY4DV1_STATE_BACKFACE )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			float z = poly->tlist[j].z;
			poly->tlist[j].x = cam->view_dist * poly->tlist[j].x / z;
			poly->tlist[j].y = cam->view_dist * poly->tlist[j].y * cam->aspect_ratio / z;
			poly->tlist[j].z = 1.0f;
		}
	}
}

void Convert_From_Homogeneous4D_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list )
{
		for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];
		
		if ( poly == NULL || !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_CLIPPED ||
			poly->state & POLY4DV1_STATE_BACKFACE )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			float w = poly->tlist[j].w;
			poly->vlist[j].x /= w;
			poly->vlist[j].y /= w;
			poly->vlist[j].z /= w;
			poly->vlist[j].w = 1.0f;
		}
	}
}

void Perspective_To_Screen_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam )
{
	float alpha = ( 0.5f * cam->viewport_width - 0.5f );
	float beta = ( 0.5f * cam->viewport_height - 0.5f );
	for ( int i = 0; i < obj->num_vertices; i++ )
	{
		obj->vlist_trans[i].x = alpha + obj->vlist_trans[i].x * alpha;
		obj->vlist_trans[i].y = beta - obj->vlist_trans[i].y * beta;
	}
}

void Perspective_To_Screen_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, CAMERA4DV1_PTR cam )
{
	float alpha = ( 0.5f * cam->viewport_width - 0.5f );
	float beta = ( 0.5f * cam->viewport_height - 0.5f );

	for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];

		if ( poly == NULL || !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_CLIPPED ||
			poly->state & POLY4DV1_STATE_BACKFACE )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			poly->tlist[j].x = alpha + alpha * poly->tlist[j].x;
			poly->tlist[j].y = beta + beta * poly->tlist[j].y;
		}
	}
}

void Camera_To_Perspective_Screen_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam )
{
	float alpha = ( cam->viewport_width * 0.5f - 0.5f );
	float beta = ( cam->viewport_height * 0.5f - 0.5f );
	for ( int i = 0; i < obj->num_vertices; i++ )
	{
		VECTOR4D_PTR v = &obj->vlist_trans[i];

		v->x = cam->view_dist *  v->x / v->z;
		v->y = cam->view_dist * v->y * cam->aspect_ratio / v->z;

		v->x = alpha + v->x * alpha;
		v->y = beta - v->y * beta;
	}
}

void Camera_To_Perspective_Screen_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, CAMERA4DV1_PTR cam )
{
	float alpha = ( cam->viewport_width * 0.5f - 0.5f );
	float beta = ( cam->viewport_height * 0.5f - 0.5f );

	for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];

		if ( poly == NULL || !(poly->state & POLY4DV1_STATE_ACTIVE )
			|| poly->state & POLY4DV1_STATE_BACKFACE
			|| poly->state & POLY4DV1_STATE_CLIPPED )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			VECTOR4D_PTR v = &poly->tlist[j];

			v->x = cam->view_dist *  v->x / v->z;
			v->y = cam->view_dist * v->y * cam->aspect_ratio / v->z;

			v->x = alpha + v->x * alpha;
			v->y = beta - v->y * beta;
		}
	}
}