// Author:Ana Butanda 
// Date: Spring 2017
// Purpose: Help screen for Armed and Ready 
//
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <GL/glx.h>
extern"C"{
#include "fonts.h"
}
extern int xres;
extern int yres;
using namespace std;
void print_Ana()
{
    string ana ="ana" ;
    cout << ana << endl;

}

void ana_show_help()
{
    Rect r;
    //float wid =120.f;
    glColor3f(1.0,1.0,1.0);
    
    glBegin(GL_QUADS);
    glVertex2i((xres/3),(yres/3));
    glVertex2i((xres/3),(yres/3)*2);
    glVertex2i((xres/3)*2,(yres/3)*2);
    glVertex2i((xres/3)*2,(yres/3));	    
    glEnd();
    r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint8b(&r,16,0x00ff0000, "Help Screen- Prototype");
    //
    ggprint8b(&r, 16,0x00ff000,"Press Hto toggle");

}

