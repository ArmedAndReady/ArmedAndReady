// Author: Mark Felisilda
// Date: Spring 2017
// Purpose: Image Conversion/Animation for Armed and Ready

#include<iostream>
#include<cstring>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<math.h>
#include<X11/Xlib.h>
#include<X11/keysym.h>
#include<GL/glx.h>
#include"log.h"
#include"ppm.h"
#include"fonts.h"
using namespace std;

Ppmimage *octoImage=NULL;
GLuint octoTexture;

void initCharMark(void)
{
    system("convert ./Game_Tiles/octopus.png ./Game_Tiles/octopus.ppm");
    octoImage = ppm6GetImage("./Game_Tiles/octopus.ppm");

    int w = octoImage->width;
    int h = octoImage->height;
    glBindTexture(GL_TEXTURE_2D, octoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	    		GL_RGB, GL_UNSIGNED_BYTE, octoImage->data);
   
   cout << "This is where the character art goes\n"; 
}

void print_Mark()
{
	string Mark = "Mark\n";
	cout << Mark;
}
