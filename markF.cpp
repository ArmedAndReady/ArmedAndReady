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
#include "fonts.h"
using namespace std;

Ppmimage *octoImage=NULL;
GLuint octoTexture;

void initCharMark()
{
//    string debug = "Printing character to screen...\n";
//    cout << debug;

    system("convert ./Game_Tiles/octopus.png ./Game_Tiles/octopus.ppm");
    octoImage = ppm6GetImage("./Game_Tiles/octopus.ppm");
    glGenTextures(1, &octoTexture);

    int w = octoImage->width;
    int h = octoImage->height;
    glColor3f(0.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glEnable(GL_TEXTURE_2D);

    glVertex2f(-64.0f, -64.0f);
    glVertex2f(-64.0f,   0.0f);
    glVertex2f(  0.0f,   0.0f);
    glVertex2f(  0.0f, -64.0f);

    glBindTexture(GL_TEXTURE_2D, octoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, octoImage->data);
    glEnd();

 //04/18 (11:52am): does image conversion, draws character tile, 
 // but does not put texture on tile   
}

void print_Mark()
{
	string Mark = "Mark\n";
	cout << Mark;
}
