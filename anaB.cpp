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
    glVertex2i((xres-100),(100));
    glVertex2i((xres-100),(yres-100));
    glVertex2i((100),(yres-100));
    glVertex2i((100),(100));	    
    glEnd();
    r.bot = 750;
    r.left = 600;
    r.center = 0;
    ggprint8b(&r,16,0x00ff0000, "Help Menu");
    ggprint8b(&r, 16,0x00ff000,"Press H to toggle");
    Shape box[4];
    glColor3i(255,0,0);
    for (int i=0; i<4;i++){
    	box[i].width=100;
    	box[i].height=10;
    	box[i].center.x =xres/2-3*i;
    	box[i].center.y =yres/2-3*i;
    }
    int offset =105;
    int offsetx =150;
    int offsety = 200;
    for (int i=0;i<5;i++){
    glColor3f(0.0,0,1.0/i);
    glBegin(GL_QUADS);
    glVertex2i((xres-offsetx),((offsety)*3)- i*offset);
    glVertex2i((xres-offsetx),(yres-offsety)-i*offset);
    glVertex2i((offsetx),(yres-offsety)-i*offset);
    glVertex2i((offsetx),((offsety)*3)-i*offset);	    
    glEnd();
    }

    r.bot= box[0].center.y+190;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000099,"Controls");
    r.bot= box[0].center.y+100;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000099,"How to ");
    r.bot= box[0].center.y+190;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000099,"option 3");
    r.bot= box[0].center.y+190;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000099,"option 4");
}
//need to draw boxes and add menu options. 
//link to a webpage
//http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php
