//testing how to use cpp files to connect to main file
//for group project

#include <iostream>
//#include <GL/glx.h>
#include "fonts.h"

using namespace std;

void show_menu()
{
	Rect r;
	r.bot = 400;
	r.left = 400;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "Start Menu");	
}

void show_help()
{
	Rect r;
	r.bot = 400;
	r.left = 400;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "Help Screen");	
}
