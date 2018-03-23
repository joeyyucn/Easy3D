#include <Windows.h>
#include <assert.h>
#include <stddef.h>
#include "render_cmd.h"
#define RENDERCMD_BUFFER_SIZE (2 * 1024 * 1024)
RenderCmdArray g_renderCmd;

bool render_cmd_init()
{
	g_renderCmd.cmds = NULL;
	g_renderCmd.usedTotal = 0;
	g_renderCmd.lastCmd = NULL;
	byte* buffer = static_cast<byte*>( VirtualAlloc( NULL, RENDERCMD_BUFFER_SIZE, MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE ) );
	if ( buffer == NULL )
	{
		return false;
	}

	/*
	if ( !VirtualAlloc( buffer, RENDERCMD_BUFFER_SIZE, MEM_COMMIT | MEM_LARGE_PAGES, PAGE_READWRITE ) )
	{
		VirtualFree( buffer, 0, MEM_RELEASE ); 
		return false;
	}
	*/

	g_renderCmd.cmds = buffer;
	g_renderCmd.usedTotal = 0;
	return true;
}

void render_cmd_shutdown()
{
	VirtualFree( g_renderCmd.cmds, 0,  MEM_RELEASE );
}

void render_cmd_reset()
{
	g_renderCmd.usedTotal = 0;
	g_renderCmd.lastCmd = NULL;
}

static void* GetCommandBuffer( RenderCommand cmd, int size )
{
	assert( ( size & 3 ) == 0 );
	RenderCMDHeader* cmdHeader = NULL;
	int freeMems = RENDERCMD_BUFFER_SIZE - g_renderCmd.usedTotal;
	
	if ( freeMems < size )
	{
		return NULL;
	}

	cmdHeader = reinterpret_cast<RenderCMDHeader*>( g_renderCmd.cmds + g_renderCmd.usedTotal );
	cmdHeader->id = cmd;
	cmdHeader->byteCount = size;
	g_renderCmd.usedTotal +=  size;
	g_renderCmd.lastCmd = cmdHeader;

	return cmdHeader;
}

void AddCmdDrawText( const TCHAR* text, int x, int y )
{
	int size;
	RenderCmdDrawText* cmd;
	
	int len = wcslen( text );
	len = len*sizeof(TCHAR);
	size = offsetof(RenderCmdDrawText, text[len+1]); 
	size = ( size + 3 ) & ~3;
	if ( !( cmd = static_cast<RenderCmdDrawText*>( GetCommandBuffer( RENDER_CMD_DRAW_TEXT, size ) ) ) )
	{
		return ;
	}

	cmd->x = x;
	cmd->y = y;
	memcpy( cmd->text, text, len );
	cmd->text[len+1] = '\0';
};
