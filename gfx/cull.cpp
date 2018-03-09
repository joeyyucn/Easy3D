#include "cull.h"

int Cull_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam, int cull_mode )
{
	POINT4D sphere_pos;
	VECTOR4D_Mul_MATRIX4X4( &obj->world_pos, &cam->mcam, &sphere_pos );
	if ( cull_mode & CULL_OBJECT_Z_PLANE )
	{
		if ( ( ( sphere_pos.z + obj->max_radius ) < cam->near_clip_z ) ||
			( ( sphere_pos.z - obj->max_radius ) > cam->far_clip_z ) )
		{
			obj->state |= OBJECT4DV1_STATE_CULLED;
			return 1;
		}
	}

	if ( cull_mode & CULL_OBJECT_X_PLANE )
	{
		float ztest = sphere_pos.z * cam->viewplane_width * 0.5f / cam->view_dist;

		if ( ( ( sphere_pos.x + obj->max_radius ) < -ztest ) ||
			( ( sphere_pos.x - obj->max_radius ) > ztest ) )
		{
			obj->state |= OBJECT4DV1_STATE_CULLED;
			return 1;
		}
	}

	if ( cull_mode & CULL_OBJECT_Y_PLANE )
	{
		float ztest = sphere_pos.z * cam->viewplane_height * 0.5f / cam->view_dist;

		if ( ( ( sphere_pos.y + obj->max_radius ) < -ztest ) ||
			( ( sphere_pos.y	- obj->max_radius ) > ztest ) )
		{
			obj->state |= OBJECT4DV1_STATE_CULLED;
			return true;
		}
	}

	return 0;
}

void Reset_OBJECT4DV1( OBJECT4DV1_PTR obj )
{
	RESET_BIT( obj->state, OBJECT4DV1_STATE_CULLED );
	
	for ( int poly = 0; poly < obj->num_polys; poly++ )
	{
		POLY4DV1_PTR curr_poly = &obj->plist[poly];

		if ( !( curr_poly->state & POLY4DV1_STATE_ACTIVE ) )
		{
			continue;
		}

		RESET_BIT( curr_poly->state, POLY4DV1_STATE_CLIPPED );
		RESET_BIT( curr_poly->state, POLY4DV1_STATE_BACKFACE );
	}
}

void Remove_Backfaces_OBJECT4DV1( OBJECT4DV1_PTR obj, CAMERA4DV1_PTR cam )
{
	if ( obj->state & OBJECT4DV1_STATE_CULLED )
	{
		return ;
	}

	for ( int i = 0; i < obj->num_polys; i++ )
	{
		POLY4DV1_PTR poly = &obj->plist[i];

		if ( !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			( poly->attr & POLY4DV1_ATTR_2SIDE ) ||
			( poly->state & POLY4DV1_STATE_CLIPPED ) ||
			( poly->state & POLY4DV1_STATE_BACKFACE ) )
		{
			continue;
		}
		
		int vindex_0 = poly->vindex[0];
		int vindex_1 = poly->vindex[1];
		int vindex_2 = poly->vindex[2];

		VECTOR4D u, n, v;
		VECTOR4D_SUB( &poly->vlist[vindex_1], &poly->vlist[vindex_0], &u );
		VECTOR4D_SUB( &poly->vlist[vindex_2], &poly->vlist[vindex_0], &v );
		VECTOR4D_CROSS( &u, &v, &n );

		VECTOR4D view;
		VECTOR4D_SUB( &cam->pos, &poly->vlist[vindex_0], &view );

		float dp = VECTOR4D_DOT( &view, &n );
		if( dp <= 0.0f )
		{
			SET_BIT( poly->state, POLY4DV1_STATE_BACKFACE );
		}
	}
}

void Remove_Backfaces_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, CAMERA4DV1_PTR cam )
{
	for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];

		if ( poly == NULL || 
			!( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			( poly->attr & POLY4DV1_ATTR_2SIDE ) ||
			( poly->state & POLY4DV1_STATE_CLIPPED ) ||
			( poly->state & POLY4DV1_STATE_BACKFACE ) )
		{
			continue;
		}

		VECTOR4D u, n, v;
		VECTOR4D_SUB( &poly->tlist[1], &poly->tlist[0], &u );
		VECTOR4D_SUB( &poly->tlist[2], &poly->tlist[0], &v );
		VECTOR4D_CROSS( &u, &v, &n );

		VECTOR4D view;
		VECTOR4D_SUB( &cam->pos, &poly->tlist[0], &view );

		float dp = VECTOR4D_DOT( &view, &n );
		if( dp <= 0.0f )
		{
			SET_BIT( poly->state, POLY4DV1_STATE_BACKFACE );
		}
	}
}