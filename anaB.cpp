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
struct Vec
{
    float x,y,z;
};
struct Shape
{
    float width, height;
    float radius;
    Vec center;
};
void ana_show_help()
{
    Rect r;
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex2i((xres/3),(yres/3));
    glVertex2i((xres/3),(yres/3)*2);
    glVertex2i((xres/3)*2,(yres/3)*2);
    glVertex2i((xres/3)*2,(yres/3));	    
    glEnd();
    r.bot = 570;
    r.left = 550;
    r.center = 0;
    ggprint8b(&r,16,0x00ff0000, "Help Menu");
    ggprint8b(&r, 16,0x00ff000,"Press H to toggle");
    Shape box[4];
    glColor3i(255,0,0);
    for (int i=0; i<4;i++){
    	box[i].width=100;
    	box[i].height=10;
    	box[i].center.x =xres/2;
    	box[i].center.y =yres/2;
    }
    r.bot= box[0].center.y-11;
    r.left =box[0].center. x-38;
    ggprint12(&r,0,0x00000099,"Controls");
}
//need to draw boxes and add menu options. 
//link to a webpage
//
