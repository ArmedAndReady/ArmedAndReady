//testing how to use cpp files to connect to main file
//for group project

#include <iostream>
//#include <GL/glx.h>
extern "C" {
#include "fonts.h"
}

using namespace std;

void show_menu()
{
	Rect r;
	r.bot = 400;
	r.left = 400;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "Start Menu-Prototype");	
	ggprint8b(&r, 16, 0x00ff0000, "Press S to start");	
}

void show_help()
{
	Rect r;
	r.bot = 400;
	r.left = 400;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "Help Screen-Prototype");
	//longer than 80 characters needs fixing	
	ggprint8b(&r, 16, 0x00ff0000, "Press H to toggle");	
}
