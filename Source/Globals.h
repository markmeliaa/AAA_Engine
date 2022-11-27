#ifndef __Globals_H__
#define __Globals_H__

#include <windows.h>
#include <stdio.h>

#define D_LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Conversion --------------
#define DEGTORAD 0.01745329251f
#define RADTODEG 1.0F / DEGTORAD

// Configuration -----------
#define BASE_SCREEN_WIDTH 1200 // previous 640
#define BASE_SCREEN_HEIGHT 700 // previous 480
#define FULLSCREEN false
#define BORDERLESS false
#define RESIZABLE false
#define FULLSCREEN_DSKTP false
#define VSYNC true
#define TITLE "\"UNREAL\" Engine"
#define GLSL_VERSION "#version 440"

#endif // __Globals__H_