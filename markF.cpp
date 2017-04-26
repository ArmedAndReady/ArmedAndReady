// Author: Mark Felisilda
// Date: Spring 2017
// Purpose: Image Conversion/Animation for Armed and Ready

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
#include "global.h"
extern "C" {
#include "fonts.h"
}

using namespace std;

Ppmimage *octoImage=NULL;
Ppmimage *redImage=NULL;
GLuint octoTexture;
GLuint rOTex;

extern int xres;
extern int yres;

void mark_show_charsel();
void check_keys(XEvent *e);
void initCharMark();
void redOctober();//use this to draw enemy
void texGen();

void mark_show_charsel()
{
    Rect r;
    glColor3ub(0,255,0);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(0,yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
    r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint8b(&r, 16, 0x0000ff, "Testing");
    /*XEvent e;
      check_keys(XEvent *e);*/
    }
/*
   int check_keys(XEvent *e)
   {
   int key = XLookupKeysym(&e->xkey, 0);
   switch(key) {
   case XK_1:
   initCharMark();
   break;
   case XK_2:
   blueOcto();
   break;
   }
   return 0;
   }*/

void initCharMark()
{
    //    string debug = "Printing character to screen...\n";
    //    cout << debug;


    int w = octoImage->width;
    int h = octoImage->height;
    float wid = 64.0f;
    //    glColor3f(0.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, octoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, octoImage->data);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();

    // 04/18 (11:52): does image conversion, draws character tile, 
    // but does not put texture on tile  
    // 04/18 (13:06): tile textured 
    // comment: I've worked on this but github hasn't been
    // recording my pushes to origin master. You can check the logs. 
    // Mark Felisilda has pushed/committed multiple times
    // 4/24 (08:26): tabs should be fixed
    // in case:
    // 		shift+V (enters visual line)
    // 		scroll down to bottom to highlight everything
    // 		"<" to remove indents
    // 		"." to repeat in case
    // 		"gg=G"
    //	unlink("./Game_Tiles/octopus.ppm");
}

void redOctober()
{
    //    string debug = "Printing character to screen...\n";
    //    cout << debug;

    int w = redImage->width;
    int h = redImage->height;
    float wid = 32.0f;
    glColor3f(1.0f, 0.0f, 0.0f);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, rOTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, redImage->data);

    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();
}

void texGen()
{
    //Player character ppm creation
    system("convert ./Game_Tiles/octopus.png ./Game_Tiles/octopus.ppm");
    octoImage = ppm6GetImage("./Game_Tiles/octopus.ppm");
    glGenTextures(1, &octoTexture);
    unlink("./Game_Tiles/octopus.ppm");

    //Enemy character ppm creation
    system("convert ./Game_Tiles/octopus.png ./Game_Tiles/redOctober.ppm");
    redImage = ppm6GetImage("./Game_Tiles/redOctober.ppm");
    glGenTextures(1, &rOTex);
    unlink("./Game_Tiles/redOctober.ppm");
}

void print_Mark()
{
    string Mark = "Mark\n";
    cout << Mark;
}
