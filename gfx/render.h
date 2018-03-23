#pragma once
#include <Windows.h>
#include "common_vector.h"
#include "render_types.h"

int render_init( HWND hwnd, int width, int height );

void render_shutdown();

void render_begin_draw();

void render_end_draw();

void render_present();

void render_clear();

void draw_line( int x1, int y1, int x2, int y2, int color );

void draw_line( POINT2DI_PTR start, POINT2DI_PTR end, int color );

void draw_OBJECT4DV1( OBJECT4DV1_PTR obj );

void draw_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list );

void draw_text( const TCHAR* text, float x, float y );

template<class Interface>
FORCEINLINE void SafeRelease( Interface** ppInterfaceToRelease )
{
	if ( *ppInterfaceToRelease )
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}