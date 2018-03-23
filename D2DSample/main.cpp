#include <Windows.h>
#include "DemoApp.h"

int WINAPI WinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow )
{
	CoInitialize( NULL );
	DemoApp game;
	if ( SUCCEEDED( game.Init( hInstance ) ) )
	{
		game.RunMessageLoop();
		game.Shutdown();
	}
	CoUninitialize();
	return 1;
}