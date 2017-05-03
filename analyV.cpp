//Author: Analy Velazquez
//Created: February 26, 2017
//Program: analyV.cpp
//Game: Armed and Ready
//Purpose: 	To make a menu screen for Armed and Ready 
//		video game. 

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <cstdlib>  
#include <unistd.h> 
#include <ctime> 
#include <cmath>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <GL/glx.h>
extern "C" {
#include "fonts.h"
}
#include "ppm.h"
#include "log.h"
#include "global.h"

extern int xres;
extern int yres;
extern Ppmimage *name_image;
extern GLuint name_texture;

extern Ppmimage *name_image2;
extern GLuint name_texture2;
using namespace std;

typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix [4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
const double physicsRate = 1.0 / 30.0;

void Analy_init();

void print_Analy() 
{
    string analy= "analy";
    cout << analy << endl;
}

struct Vec1 {
    float x, y, z;
};

struct Shape {
    float width, height;
    float radius;
    Vec1 center;
};

typedef struct Bubble {
    Vec pos;
    Vec lastpos;
    Vec vel;
    Vec maxvel;
    Vec force;
    float radius;
    float color[3];
} Bubble;

const int MAX_BUBBLES = 1000;
Bubble bubble[MAX_BUBBLES];
int nbubbles=0;

typedef struct t_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
}Button;

Button button1[4];

void Analy_init()
{
    int offsets1[4];
    int top1[4];
    int bottom1[4];

    int offset2 = 90;

    for (int i=0; i<4;i++) {
	bottom1[i]= (450-(i*offset2));
	top1[i] = (520-(i*offset2));
	float middle[4];
       middle[i] = (top1[i] + bottom1[i])/2;
	offsets1[i] = middle[i];
    }

    int nbuttons = 0;
	
    //first button
    button1[nbuttons].r.width = 200;
    button1[nbuttons].r.height = 70;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = offsets1[4]; 
    button1[nbuttons].r.left = 70;
    button1[nbuttons].r.right = xres - 70;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.5;
    button1[nbuttons].dcolor[1] = 0.4f;
    button1[nbuttons].dcolor[2] = 0.7f;
    
    //second button
    nbuttons++;
    button1[nbuttons].r.width = 200;
    button1[nbuttons].r.height = 70;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = offsets1[3]; 
    button1[nbuttons].r.left = 70;
    button1[nbuttons].r.right = xres-70;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.5;
    button1[nbuttons].dcolor[1] = 0.4f;
    button1[nbuttons].dcolor[2] = 0.7f;

    //third button
    nbuttons++;
    button1[nbuttons].r.width = 200;
    button1[nbuttons].r.height = 70;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = offsets1[4]; 
    button1[nbuttons].r.left = 70;
    button1[nbuttons].r.right = xres-70;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.5;
    button1[nbuttons].dcolor[1] = 0.4f;
    button1[nbuttons].dcolor[2] = 0.7f;

    //fourth button
    nbuttons++;
    button1[nbuttons].r.width = 200;
    button1[nbuttons].r.height = 70;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = offsets1[4]; 
    button1[nbuttons].r.left = 70;
    button1[nbuttons].r.right = xres-70;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.5;
    button1[nbuttons].dcolor[1] = 0.4f;
    button1[nbuttons].dcolor[2] = 0.7f;

}
void Analy_show_menu()
{
    //this will be the background (vertix have to be in order
    //(0,0), (0,y), (x,y), (x,0) 
    // glColor3ub(0, 204, 204);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();

    glColor3ub(0, 204, 204);
    glBindTexture(GL_TEXTURE_2D, name_texture2);
    glPushMatrix();
    glBegin(GL_QUADS);
    //the original image had the dimensions 725x433 
    //then we used a canvas to set it up and new dimensions are 1024x512
    //we always want to use a power of 2
    float w = (725.0/1024.0);
    float h = (433.0/512.0);
    glTexCoord2f(0.0, h); glVertex2i(0, 0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0, yres);
    glTexCoord2f(w, 0.0); glVertex2i(xres, yres);
    glTexCoord2f(w, h); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();

    //physics function without being a function
    //create a bubble
    if (nbubbles < MAX_BUBBLES) {
	bubble[nbubbles].pos[0] = rnd() * (xres) ;
	bubble[nbubbles].pos[1] = (float)yres/2.0 - 600.0 ;
	VecZero(bubble[nbubbles].vel);
	VecZero(bubble[nbubbles].force);
	//bubble radius
	bubble[nbubbles].radius = rnd() * 12.0 + 2.0;
	bubble[nbubbles].color[0] = 1.0;
	bubble[nbubbles].color[1] = 1.0;
	bubble[nbubbles].color[2] = 1.0;
	++nbubbles;
    }

    //move bubbles
    for (int i=0; i<nbubbles; i++) {
	bubble[i].force[0] = rnd() * 0.5 - 0.25;
	bubble[i].force[1] = 0.0001 + bubble[i].radius*0.005;
	bubble[i].vel[0] += bubble[i].force[0];
	bubble[i].vel[1] += bubble[i].force[1];
	//constrain the x-movement of a bubble.
	if (bubble[i].vel[0] > 1.0)
	    bubble[i].vel[0] = 0.9;
	if (bubble[i].vel[0] < -1.0)
	    bubble[i].vel[0] = -1.0;
	bubble[i].pos[0] += bubble[i].vel[0];
	bubble[i].pos[1] += bubble[i].vel[1];
	//radius will increase a little bit over its life
	bubble[i].radius *= 1.002;
	//if float to the top??
	if (bubble[i].pos[1] > yres+20) {
	    //delete from array
	    --nbubbles;
	    bubble[i] = bubble[nbubbles];
	}
	//random bubble pop
	if (rand() < 1000000) {
	    int bnum = rand() % nbubbles;
	    --nbubbles;
	    bubble[bnum] = bubble[nbubbles];
	
	}
    }
    //render function without being a function
    static int firsttime=1;
    const int npts=12;
    static float vert[npts][2];
    if (firsttime) {
	firsttime=0;
	double ang = 0.0;
	double inc = (3.14159265 * 2.0) / (double)npts;
	for (int i=0; i<npts; i++) {
	    vert[i][0] = cos(ang);
	    vert[i][1] = sin(ang);
	    ang += inc;
	}
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    //draw bubbles
    for (int i=0; i<nbubbles; i++) {
	glPushMatrix();
	glTranslated(bubble[i].pos[0], bubble[i].pos[1], 0.0);
	float r;
	glColor3fv(bubble[i].color);
	glBegin(GL_LINE_STRIP);
	r = bubble[i].radius;
	for (int j=0; j<npts; j++) {
	    glVertex2f(vert[j][0]*r, vert[j][1]*r);
	}
	glVertex2f(vert[0][0]*r, vert[0][1]*r);
	glEnd();
	glBegin(GL_POINTS);
	r = bubble[i].radius * 0.4;
	glVertex2f(-r, r);
	glVertex2f(-r-1, r);
	glVertex2f(-r-1, r-1);
	glEnd();
	glPopMatrix();
    }

    //logo menu title is created here
    glColor3ub(0, 204, 204);
    glBindTexture(GL_TEXTURE_2D, name_texture);
    glPushMatrix();
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex2i(190, yres-100);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres-190, yres-100);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres-190, 680);
    glTexCoord2f(0.0, 1.0); glVertex2i(190, 680);
    glEnd();
    glPopMatrix();

    //menu boxes are created here
    //i is set to 4 to create 4 boxes
    
	glColor3i(9, 60, 235);
    for(int i=0; i<4; i++){
	if (button1[i].over) {
	    glLineWidth(2);
	    glBegin(GL_LINE_LOOP);
	    glVertex2i(button1[i].r.left-2, button1[i].r.bot-2);
	    glVertex2i(button1[i].r.left-2, button1[i].r.top+2);
	    glVertex2i(button1[i].r.right+2, button1[i].r.top+2);
	    glVertex2i(button1[i].r.right+2, button1[i].r.bot-2);
	    glVertex2i(button1[i].r.left-2, button1[i].r.bot-2);
	glEnd();
	glLineWidth(1);
	glColor3fv(button1[i].dcolor);
	}
	else {
	    glColor3f(0.0,1.0/(2*i),2.0);
	}
	
	int offset1=90;
	int offsetx1=500;
	int offsety1=200;
//	glColor3ub(9, 60, 235);
	//creates 3 boxes here 
	//vertices for box 1: 	(725, 450)
	//			(725, 520)
	//			(525, 520)
	//			(525, 450)
	//vertices for box 2: 	(725, 360)
	//			(725, 430)
	//			(525, 430)
	//			(525, 360)
	//vertices for box 3: 	(725, 270)
	//			(725, 340)
	//			(525, 340)
	//			(525, 270)
	//vertices for box 4: 	(725, 180)
	//			(725, 250)
	//			(525, 250)
	//			(525, 180)
//	glPushMatrix();
//	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glVertex2i((xres-offsetx1-25),((offsety1)*3)- i*offset1-150);
	glVertex2i((xres-offsetx1-25),(yres-offsety1-30)-i*offset1-150);
	glVertex2i((offsetx1+25),(yres-offsety1-30)-i*offset1-150);
	glVertex2i((offsetx1+25),((offsety1)*3)-i*offset1-150);
	glEnd();
	//glPopMatrix();
    }

    Rect r;

    //menu boxes names are created here
    //so each one has a position to match to be
    //centered in each box
    
    /*r.bot = 475; 
    r.left =605 ;
    r.center = 0; 
    */
    r.bot = button1[0].r.centery + 475;
    r.left = button1[0].r.centerx + 605;
    ggprint16(&r, 16, 0x00ffffff, "Start");

    /*
    r.bot = 385;
    r.left =605 ;
    r.center = 0;
    */
    r.bot = button1[0].r.centery + 385;
    r.left = button1[0].r.centerx+605;
    ggprint16(&r, 16, 0x00ff0000, "Help");

    /*
    r.bot = 295; 
    r.left =580 ;
    r.center = 0;
    */
    r.bot = button1[0].r.centery + 295;
    r.left = button1[0].r.centerx + 580;
    ggprint16(&r, 0, 0x00ff0000, "High Score");


    /*
    r.bot = 205; 
    r.left =605 ;
    r.center = 0;
    */
    r.bot = button1[0].r.centery + 205;
    r.left = button1[0].r.centerx + 605;
    ggprint16(&r, 0, 0x00ff0000, "Quit");

    r.bot = 30;
    r.left = 250;
    r.center = 0;
    ggprint12(&r, 12, 0x00fffffff, "Created by: Ana Butanda,"\
	    " Mark Felisilda, Esteban Lopez, Analy Velazquez -"\
	    "CMPS 3350 [Software Engineering] Spring 2017");

}

/*void drawbuttons(Game *g) 
  {
  Shape *s;
//    s = &game->



}*/

