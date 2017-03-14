//Analy Velazquez
//Armed and Ready
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <GL/glx.h>
extern "C" {
#include "fonts.h"
}
#include "ppm.h"

extern int xres;
extern int yres;
extern Ppmimage *name_image;
extern GLuint name_texture;

using namespace std;
void print_Analy() 
{
    string analy= "analy";
    cout << analy << endl;
}
void Analy_show_menu()
{
    Rect r;
/*
//this will be the background (vertix have to be in order
//(0,0), (0,y), (x,y), (x,0)
glColor3ub(0, 204, 204);
glPushMatrix();
glBegin(GL_QUADS);
glVertex2i(0, 0);
glVertex2i(0, yres);
glVertex2i(xres, yres);
glVertex2i(xres, 0);
glEnd();
glPopMatrix();
*/
//glColor3ub(0, 204, 204);
glBindTexture(GL_TEXTURE_2D, name_texture);
glPushMatrix();
glBegin(GL_QUADS);
glTexCoord2f(0.0, 1.0); glVertex2i(190, 385);
glTexCoord2f(0.0, 0.0); glVertex2i(190, yres-385);
glTexCoord2f(1.0, 0.0); glVertex2i(xres-190, yres-385);
glTexCoord2f(1.0, 1.0); glVertex2i(xres-190, 385);
glEnd();
glPopMatrix();


r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "Armed & Ready");
    ggprint16(&r, 16, 0x00ffffff, "Main Menu");
    ggprint16(&r, 16, 0x00ffffff, "Play");
    ggprint16(&r, 16, 0x00ff0000, "Help");
    ggprint16(&r, 16, 0x00ff0000, "Rules");
    ggprint8b(&r, 16, 0x00ff0000, "Press S to start");
}

