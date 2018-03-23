#pragma once

enum RenderCommand
{
	RENDER_CMD_NONE,
	RENDER_CMD_DRAW_TEXT,
};

struct RenderCMDHeader
{
	unsigned short id;
	unsigned short byteCount;
};

struct RenderCmdArray
{
	byte* cmds;
	int usedTotal;
	RenderCMDHeader* lastCmd;
};

struct RenderCmdDrawText
{
	RenderCMDHeader	header;
	unsigned short x;
	unsigned short y;
	char text[3];
};
bool render_cmd_init();
void render_cmd_shutdown();
void render_cmd_reset();

void AddCmdDrawText( const TCHAR* text, int x, int y );