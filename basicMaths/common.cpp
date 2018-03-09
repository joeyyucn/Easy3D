#include "common.h"
#include <stdarg.h>
#include <stdio.h>

void Com_Printf( const char* fmt, ... )
{
	va_list argptr;
	char msg[MAX_PRINT_MSG];

	va_start( argptr, fmt );
	vsnprintf( msg, MAX_PRINT_MSG, fmt, argptr );
	va_end( argptr );

	printf( msg );
}