#include "common_sys.h"

DWORD start_clock_count = 0;
DWORD Get_Clock()
{
	return GetTickCount();
}

DWORD Start_Clock()
{
	return ( start_clock_count = GetTickCount() );
}

DWORD Wait_Clock( DWORD count )
{
	while ( ( GetTickCount() - start_clock_count ) < count )
	{
		Sleep(1);
	}
	return GetTickCount();
}
