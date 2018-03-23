#include "render.h"
#include <wincodec.h>
#include <d2d1.h>
#include <dwrite.h>
#include <assert.h>
#include "render_types.h"
#include "render_cmd.h"

ID2D1Factory* g_pD2D1Factory = NULL;
ID2D1HwndRenderTarget* g_pRenderTarget = NULL;
ID2D1SolidColorBrush* g_pRedBrush = NULL;

IWICImagingFactory* g_pWICFactory = NULL;
IWICBitmap* g_pWICBitmap = NULL;
IWICBitmapLock* g_pBitmapLock = NULL;

IDWriteFactory* g_pDWriteFactory = NULL;
IDWriteTextFormat* g_pTextFormat = NULL;

UCHAR* g_pBackBuffer = NULL;
UINT g_pBackBufferSize = 0;
UINT g_backbufferStride = 0;
UINT g_viewportWidth = 0;
UINT g_viewportHeight = 0;

#define ASSERT_BACKBUFFER_UNLOCK	assert( g_pBackBuffer != NULL )

int render_init( HWND hwnd, int width, int height )
{
	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2D1Factory );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	D2D1_SIZE_U size = D2D1::SizeU( width, height );
	hr = g_pD2D1Factory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties( hwnd, size ), &g_pRenderTarget ); 
	if ( FAILED( hr ) )
	{
		return hr;
	}

	hr = CoCreateInstance( CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, ( LPVOID* ) &g_pWICFactory );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	hr = g_pWICFactory->CreateBitmap( width, height, GUID_WICPixelFormat32bppBGR, WICBitmapCacheOnDemand, &g_pWICBitmap );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(g_pDWriteFactory), reinterpret_cast<IUnknown**>(&g_pDWriteFactory) );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	hr = g_pDWriteFactory->CreateTextFormat( L"Verdana", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50, L"", &g_pTextFormat );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	hr = g_pRenderTarget->CreateSolidColorBrush( D2D1::ColorF(D2D1::ColorF::Red), &g_pRedBrush );
	if ( FAILED( hr ) )
	{
		return hr;
	}
	g_pTextFormat->SetTextAlignment( DWRITE_TEXT_ALIGNMENT_LEADING );
	g_pTextFormat->SetParagraphAlignment( DWRITE_PARAGRAPH_ALIGNMENT_NEAR );
	g_viewportWidth = width;
	g_viewportHeight = height;

	render_cmd_init();
	return 1;
}

void render_shutdown()
{
	SafeRelease( &g_pD2D1Factory );
	SafeRelease( &g_pRenderTarget );
	SafeRelease( &g_pWICFactory );
	SafeRelease( &g_pWICBitmap );
	SafeRelease( &g_pDWriteFactory );
	SafeRelease( &g_pTextFormat );
	SafeRelease( &g_pRedBrush );
	render_cmd_shutdown();
}

static UCHAR* lock_back_buffer()
{
	if ( g_pBackBuffer )
	{
		return g_pBackBuffer;
	}

	g_pWICBitmap->Lock( NULL, WICBitmapLockWrite, &g_pBitmapLock );
	g_pBitmapLock->GetDataPointer( &g_pBackBufferSize, &g_pBackBuffer );
	g_pBitmapLock->GetStride( &g_backbufferStride );
	return g_pBackBuffer;
}

static void unlock_back_buffer()
{
	if ( g_pBackBuffer )
	{
		SafeRelease( &g_pBitmapLock );
		g_pBackBuffer = NULL;
	}
}

void render_begin_draw()
{
	lock_back_buffer();
}

void render_end_draw()
{
	unlock_back_buffer();
}

void render_clear()
{
	ASSERT_BACKBUFFER_UNLOCK;
	memset( g_pBackBuffer, 0, g_pBackBufferSize );
}

void render_present()
{
	ID2D1Bitmap* pBitmap = NULL;
	g_pRenderTarget->CreateBitmapFromWicBitmap( g_pWICBitmap, &pBitmap );
	if ( pBitmap )
	{
		g_pRenderTarget->BeginDraw();
		g_pRenderTarget->DrawBitmap( pBitmap );
		draw_text( L"hello", 0, 0 );
		g_pRenderTarget->EndDraw();
		SafeRelease( &pBitmap );
	}
}

#define CLIP_W		0x0001
#define CLIP_E		0x0002
#define CLIP_N		0x0004
#define CLIP_S		0x0008

#define CLIP_NW		0x0005
#define CLIP_SW		0x0009
#define CLIP_NE		0x0006
#define CLIP_SE		0x000a

int clip_point_test( POINT2DI_PTR point, RECTV1_PTR rect )
{
	int flag = 0;

	if ( point->x < rect->left )
	{
		flag |= CLIP_W;
	}
	else if ( point->x > rect->right )
	{
		flag |= CLIP_E;
	}

	if ( point->y < rect->top )
	{
		flag |= CLIP_N;
	}
	else if ( point->y > rect->bottom )
	{
		flag |= CLIP_S;
	}
	return flag;
}

void clip_point( POINT2DI_PTR point, POINT2DI_PTR addpoint, RECTV1_PTR rect, int flag )
{
	switch( flag )
	{
	case CLIP_W:
		point->y = ( rect->left - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
		point->x = rect->left;
		break;
	case CLIP_E:
		point->y = ( rect->right - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
		point->x = rect->right;
		break;
	case CLIP_S:
		point->x = ( point->x - addpoint->x ) * ( rect->bottom - point->y ) / ( point->y - addpoint->y ) + point->x;
		point->y = rect->bottom;
		break;
	case CLIP_N:
		point->x = ( point->x - addpoint->x ) * ( rect->top - point->y ) / ( point->y - addpoint->y ) + point->x;
		point->y = rect->top;
		break;
	case CLIP_NW:
		{
			int newY = ( rect->left - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
			if ( newY < rect->top || newY > rect->bottom )
			{
				point->x = ( point->x - addpoint->x ) * ( rect->top - point->y ) / ( point->y - addpoint->y ) + point->x;
				point->y = rect->top;
			}
			else
			{
				point->y = newY;
				point->x = rect->left;
			}
		}
		break;
	case CLIP_NE:
		{
			int newY = ( rect->right - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
			if ( newY < rect->top || newY > rect->bottom )
			{
				point->x = ( point->x - addpoint->x ) * ( rect->top - point->y ) / ( point->y - addpoint->y ) + point->x;
				point->y = rect->top;
			}
			else
			{
				point->y = newY;
				point->x = rect->right;
			}
		}
		break;
	case CLIP_SW:
		{
			int newY = ( rect->left - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
			if ( newY < rect->top || newY > rect->bottom )
			{
				point->x = ( point->x - addpoint->x ) * ( rect->bottom - point->y ) / ( point->y - addpoint->y ) + point->x;
				point->y = rect->bottom;
			}
			else
			{
				point->y = newY;
				point->x = rect->left;
			}
		}
		break;
	case CLIP_SE:
		{
			int newY = ( rect->right - point->x ) * ( point->y - addpoint->y ) / ( point->x - addpoint->x ) + point->y;
			if ( newY < rect->top || newY > rect->bottom )
			{
				point->x = ( point->x - addpoint->x ) * ( rect->bottom - point->y ) / ( point->y - addpoint->y ) + point->x;
				point->y = rect->bottom;
			}
			else
			{
				point->y = newY;
				point->x = rect->right;
			}
		}
		break;
	}
}

int clip_line( POINT2DI_PTR start, POINT2DI_PTR end )
{
	RECTV1 region = { 0, (g_viewportWidth - 1 ), 0, (g_viewportHeight - 1) };

	int startClip = clip_point_test( start , &region );
	int endClip = clip_point_test( end, &region );
	
	if ( endClip != 0 && startClip != 0 && ( endClip & startClip ) != 0 )
	{
		// fully clipped
		return 0;
	}

	clip_point( end, start, &region, endClip );
	clip_point( start, end, &region, startClip );
	return 1;
}


void draw_line( POINT2DI_PTR start, POINT2DI_PTR end, int color )
{
	ASSERT_BACKBUFFER_UNLOCK;

	POINT2DI lstart = { start->x, start->y };
	POINT2DI lend = { end->x, end->y };
	if ( !clip_line( &lstart, &lend ) )
	{
		// the line have been fully clipped
		return;
	}

	int* pBuffer = (int*) g_pBackBuffer;
	pBuffer = ( int * ) ( ( UCHAR* ) pBuffer + lstart.y * g_backbufferStride + 4 * lstart.x );

	int xDelta = lend.x - lstart.x;
	int yDelta = lend.y - lstart.y;
	int xInc, yInc, xDelta2, yDelta2, error;
	if ( xDelta >= 0 )
	{
		xInc = 1;
	}
	else
	{
		xInc = -1;
		xDelta = -xDelta;
	}

	if ( yDelta > 0 )
	{
		yInc = g_backbufferStride;
	}
	else
	{
		yInc = -(int)g_backbufferStride;
		yDelta = -yDelta;
	}

	xDelta2 = xDelta << 1;
	yDelta2 = yDelta << 1;

	if ( xDelta > yDelta )
	{
		error = yDelta2 - xDelta;

		for ( int i = 0; i <= xDelta; i++ )
		{
			*pBuffer = color;

			if ( error >= 0 )
			{
				error -= xDelta2;
				pBuffer = (int*)( (UCHAR*) pBuffer + yInc );
			}

			error += yDelta2;
			pBuffer += xInc;
		}
	}
	else
	{
		error = xDelta2 - yDelta;
		
		for ( int i = 0; i < yDelta; i++ )
		{
			*pBuffer = color;

			if ( error >= 0 )
			{
				error -= yDelta2;
				pBuffer += xInc;
			}

			error += xDelta2;
			pBuffer = (int*)( (UCHAR*) pBuffer + yInc );
		}
	}
}

void draw_line( int x1, int y1, int x2, int y2, int color )
{
	ASSERT_BACKBUFFER_UNLOCK;

	POINT2DI lstart = { x1, y1 };
	POINT2DI lend = { x2, y2 };
	if ( !clip_line( &lstart, &lend ) )
	{
		// the line have been fully clipped
		return;
	}

	int* pBuffer = (int*) g_pBackBuffer;
	pBuffer = ( int * ) ( ( UCHAR* ) pBuffer + lstart.y * g_backbufferStride + 4 * lstart.x );

	int xDelta = lend.x - lstart.x;
	int yDelta = lend.y - lstart.y;
	int xInc, yInc, xDelta2, yDelta2, error;
	if ( xDelta >= 0 )
	{
		xInc = 1;
	}
	else
	{
		xInc = -1;
		xDelta = -xDelta;
	}

	if ( yDelta > 0 )
	{
		yInc = g_backbufferStride;
	}
	else
	{
		yInc = -(int)g_backbufferStride;
		yDelta = -yDelta;
	}

	xDelta2 = xDelta << 1;
	yDelta2 = yDelta << 1;

	if ( xDelta > yDelta )
	{
		error = yDelta2 - xDelta;

		for ( int i = 0; i <= xDelta; i++ )
		{
			*pBuffer = color;

			if ( error >= 0 )
			{
				error -= xDelta2;
				pBuffer = (int*)( (UCHAR*) pBuffer + yInc );
			}

			error += yDelta2;
			pBuffer += xInc;
		}
	}
	else
	{
		error = xDelta2 - yDelta;
		
		for ( int i = 0; i < yDelta; i++ )
		{
			*pBuffer = color;

			if ( error >= 0 )
			{
				error -= yDelta2;
				pBuffer += xInc;
			}

			error += xDelta2;
			pBuffer = (int*)( (UCHAR*) pBuffer + yInc );
		}
	}
}

void draw_OBJECT4DV1( OBJECT4DV1_PTR obj )
{
	if ( !( obj->state & OBJECT4DV1_STATE_ACTIVE ) || 
		obj->state & OBJECT4DV1_STATE_CULLED )
	{
		return;
	}

	for ( int i = 0; i < obj->num_polys; i++ )
	{
		POLY4DV1_PTR poly = &obj->plist[i];

		if ( !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_CLIPPED ||
			poly->state & POLY4DV1_STATE_BACKFACE )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			POINT4D_PTR pStart = &obj->vlist_trans[poly->vindex[j]];
			POINT4D_PTR pEnd = &obj->vlist_trans[poly->vindex[(j+1)%3]];

			draw_line( (int) ( pStart->x + 0.5f ), (int) ( pStart->y + 0.5f ), (int)( pEnd->x + 0.5f ), (int)( pEnd->y + 0.5f ) , 0xffffff );
		}
	}
}

void draw_RENDERLIST4DV1( RENDERLIST4DV1_PTR render_list )
{
	for ( int i = 0; i < render_list->num_polys; i++ )
	{
		POLYF4DV1_PTR poly = render_list->poly_ptrs[i];

		if ( poly == NULL || !( poly->state & POLY4DV1_STATE_ACTIVE ) ||
			poly->state & POLY4DV1_STATE_BACKFACE ||
			poly->state & POLY4DV1_STATE_CLIPPED )
		{
			continue;
		}

		for ( int j = 0; j < 3; j++ )
		{
			POINT4D_PTR start = &poly->tlist[j];
			POINT4D_PTR end = &poly->tlist[(j+1)%3];

			draw_line( (int) ( start->x + 0.5f ), (int)( start->y + 0.5f ), (int)( end->x + 0.5f ), (int)( end->y + 0.5f ), 0xffffff );
		}
	}
}

void draw_text( const TCHAR* text, float x, float y )
{
	D2D1_SIZE_F size = g_pRenderTarget->GetSize();
	D2D1_RECT_F rect = D2D1::RectF( x, y, size.width - x, size.height - y );
	g_pRenderTarget->DrawTextW( text, wcslen( text ), g_pTextFormat, &rect, g_pRedBrush );
	AddCmdDrawText( text, x, y );

}