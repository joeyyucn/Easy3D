#include "render_types.h"

void Reset_OBJECT4DV1( OBJECT4DV1_PTR obj )
{
	RESET_BIT( obj->state, OBJECT4DV1_STATE_CULLED );
	for ( int i = 0; i < obj->num_polys; i++ )
	{
		POLY4DV1_PTR poly = &obj->plist[i];
		if ( !( poly->state & POLY4DV1_STATE_ACTIVE ) )
		{
			continue;
		}

		RESET_BIT( poly->state, POLY4DV1_STATE_CLIPPED );
		RESET_BIT( poly->state, POLY4DV1_STATE_BACKFACE );
	}
}

void Reset_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list )
{
	render_list->num_polys = 0;
}

int Insert_OBJECT4DV1_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list, OBJECT4DV1_PTR obj )
{
	if ( !( obj->state & OBJECT4DV1_STATE_ACTIVE ) || 
		obj->state & OBJECT4DV1_STATE_CULLED ||
		!( obj->state & OBJECT4DV1_STATE_VISIBLE ) )
	{
		return 0;
	}

	for ( int i = 0; i < obj->num_polys; i++ )
	{
		POLY4DV1_PTR poly = &obj->plist[i];

		if ( !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_BACKFACE ||
			poly->state & POLY4DV1_STATE_CLIPPED )
		{
			continue;
		}

		int index = render_list->num_polys;
		POLYF4DV1_PTR fpoly = &render_list->poly_data[index];

		fpoly->attr = poly->attr;
		fpoly->state = poly->state;
		fpoly->color = poly->color;
		
		fpoly->pNext = NULL;
		if ( i == 0 )
		{
			fpoly->pPrev = NULL;
		}
		else
		{
			fpoly->pPrev = render_list->poly_ptrs[index - 1];
			render_list->poly_ptrs[index-1]->pPrev = fpoly;
		}

		for ( int i = 0; i < 3; i++ )
		{
			VECTOR4D_COPY( &fpoly->tlist[i], &obj->vlist_trans[poly->vindex[i]] );
			VECTOR4D_COPY( &fpoly->vlist[i], &obj->vlist_local[poly->vindex[i]] );
		}

		render_list->poly_ptrs[index] = fpoly;
		render_list->num_polys++;
	}
	return 1;
}