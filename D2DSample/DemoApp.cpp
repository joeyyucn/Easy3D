#include "DemoApp.h"
#include "file_plg.h"
#include "transform.h"
#include "cull.h"
#include "render.h"
#include "common_sys.h"

DemoApp::DemoApp(void)
{
}


DemoApp::~DemoApp(void)
{
}

HRESULT DemoApp::Init( HINSTANCE hinstance )
{
	HRESULT hr;
	WNDCLASSEX wcex = { sizeof( WNDCLASSEX ) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DemoApp::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof( LONG_PTR );
	wcex.hInstance = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor( NULL, IDI_APPLICATION );
	wcex.lpszClassName = L"D2DSample";

	RegisterClassEx( &wcex );

	m_hWnd = CreateWindow(
		L"D2DSample",
		L"Direct2D Sample App",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL, 
		NULL,
		NULL,
		this
		);

	hr = m_hWnd ? S_OK : S_FALSE;

	if( FAILED( hr ) )
	{
		return hr;
	}

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	// init render
	RECT rc;
	GetClientRect( m_hWnd, &rc );
	int viewportWidth = rc.right - rc.left;
	int viewportHeight = rc.bottom - rc.top;
	render_init( m_hWnd, viewportWidth, viewportHeight );

	// INIT CAMERA
	VECTOR4D world_pos, dir;
	VECTOR4D_INITXYZ( &world_pos, 0, 5, -5 );
	VECTOR4D_INITXYZ( &dir, 45, 0, 0 );

	Init_CAMERA4DV1( &m_camera, CAMERATYPE_EULER, &world_pos, &dir, NULL, 1, 100, 90, viewportWidth, viewportHeight );
	Build_CAM4DV1_Matrix_Euler( &m_camera, CAM_ROT_SEQ_YXZ );

	// LOAD MODEL 
	VECTOR4D_INITXYZ( &world_pos, 0, 0, 0 );
	VECTOR4D rot;
	VECTOR4D_INITXYZ( &rot, 0, 0, 0 );
	VECTOR4D scale;
	VECTOR4D_INITXYZ( &scale, 1.0f, 1.0f, 1.0f );
	if ( !plg_load_OBJECT4DV1( &m_object, "cube.plg", &world_pos, &rot, &scale ) )
	{
		return S_FALSE;
	}

	return hr;
}
LRESULT CALLBACK DemoApp::WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = 0;

	if ( message == WM_CREATE )
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT) lParam;
		DemoApp* pDemoApp = (DemoApp*) pcs->lpCreateParams;
		SetWindowLongPtr( hwnd, GWLP_USERDATA, PtrToUlong( pDemoApp ) );
	}
	else
	{
		DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(GetWindowLongPtr( hwnd, GWLP_USERDATA ));
		
		switch ( message )
		{
		case WM_SIZE:
			{
				pDemoApp->OnResize();
			}
			break;
		case WM_DISPLAYCHANGE:
			{
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint( hwnd, &ps );

				EndPaint( hwnd, &ps );
				result = 0;
			}
			break;
		case WM_DESTROY:
			{
				PostQuitMessage( 0 );
				result = 0;
			}
			break;
		default:
			result = DefWindowProc( hwnd, message, wParam, lParam );
			break;
		}

	}

	return result;
}

void DemoApp::RunMessageLoop()
{
	MSG msg;
	BOOL bRet;
	while ( true )
	{
		if ( bRet = PeekMessage( &msg, NULL, 0, 0 , PM_REMOVE ) )
		{
			if ( msg.message == WM_QUIT )
			{
				break;
			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		GameMain();
	}
}

#define USE_RENDERLIST

void DemoApp::GameMain()
{
	DWORD frame_start = Start_Clock();
#ifdef USE_RENDERLIST
	RENDERLIST4DV1 render_list;
	Reset_RENDERLIST4DV1( &render_list );
	Reset_OBJECT4DV1( &m_object );
	m_object.world_pos.x = 0;
	Model_To_World_OBJECT4DV1( &m_object );
	Insert_OBJECT4DV1_RENDERLIST4DV1( &render_list, &m_object );

	m_object.world_pos.x = 3;
	Reset_OBJECT4DV1( &m_object );
	Model_To_World_OBJECT4DV1( &m_object );
	Insert_OBJECT4DV1_RENDERLIST4DV1( &render_list, &m_object );

	Remove_Backfaces_RENDERLIST4DV1( &render_list, &m_camera );
	World_To_Camera_RENDERLIST4DV1( &m_camera, &render_list );
	Camera_To_Perspective_RENDERLIST4DV1( &render_list, &m_camera );
	Perspective_To_Screen_RENDERLIST4DV1( &render_list, &m_camera );
#else
	Model_To_World_OBJECT4DV1( &m_object );
	Remove_Backfaces_OBJECT4DV1( &m_object, &m_camera );
	World_To_Camera_OBJECT4DV1( &m_camera, &m_object );
	Camera_To_Perspective_OBJECT4DV1( &m_object, &m_camera );
	Perspective_To_Screen_OBJECT4DV1( &m_object, &m_camera );
#endif

	render_begin_draw();
#ifdef USE_RENDERLIST
	draw_RENDERLIST4DV1( &render_list );
#else
	draw_OBJECT4DV1( &m_object );
#endif
	render_end_draw();
	render_present();
	
	// FPS calc
	static DWORD time_cost = 0;
	static int frames = 0;
	DWORD frame_end = Wait_Clock(33);
	time_cost += frame_end - frame_start;
	frames++;
	if ( time_cost >= 1000 )
	{
		TCHAR log[255] = {0};
		wsprintf( log, L"FPS: %d\n", frames );
		OutputDebugString( log );
		time_cost = 0;
		frames = 0;
	}
}

void DemoApp::Shutdown()
{
	render_shutdown();
}

void DemoApp::OnResize()
{

}