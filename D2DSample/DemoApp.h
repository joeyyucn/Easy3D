#pragma once
#include <Windows.h>
#include "camera.h"

class DemoApp
{
public:
	DemoApp(void);
	~DemoApp(void);

	HRESULT Init( HINSTANCE hinstance );
	void Shutdown();
	void RunMessageLoop();

private:
	HWND m_hWnd;

	static LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
	void OnResize();

protected:
	void GameMain();

	CAMERA4DV1 m_camera;
	OBJECT4DV1 m_object;
};