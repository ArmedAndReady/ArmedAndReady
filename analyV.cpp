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
extern int state_menu;
extern int state_help;
extern int state_charsel;
extern int state_end;
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

Button button1[5];
Button button2[5];

void Analy_init()
{
    int top1[5];
    int bottom1[5];

    int offset2 = 90;
    int offsety2 = 200;
    for (int i=0; i<5;i++) {
	bottom1[i]= (offsety2*3-i*offset2-150);
	top1[i] = (yres-offsety2-30-i*offset2-150);
    }

    int nbuttons=0;
    //vertices for box 1:	(725, 450)
    //				(725, 520)
    //				(525, 520)
    //				(525, 450)
    //	y cordinate from 900 upside down : 450, 380 = 

    //vertices for box 2: 	(725, 360)
    //				(725, 430)
    //				(525, 430)
    //				(525, 360)
    //	y cordinate from 900 upside down : 540, 470 = 505

    //vertices for box 3: 	(725, 270)
    //				(725, 340)
    //				(525, 340)
    //				(525, 270)
    //	y cordinate from 900 upside down : 630, 560 = 595

    //vertices for box 4: 	(725, 180)
    //				(725, 250)
    //				(525, 250)
    //				(525, 180)
    //	y cordinate from 900 upside down : 720, 650 = 685
  
    //vertices for box 5: 	(725, 90)
    //				(725, 160)
    //				(525, 160)
    //				(525, 90)
    //	y cordinate from 900 upside down : 810, 740 =  775

    //first button
    button1[nbuttons].r.width = 100;
    button1[nbuttons].r.height = 35;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = 380; 
    button1[nbuttons].r.left =  525;
    button1[nbuttons].r.right = xres-525;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.0;
    button1[nbuttons].dcolor[1] = 0.0f;
    button1[nbuttons].dcolor[2] = 0.0f;
    //second button
    nbuttons++;
    button1[nbuttons].r.width = 100;
    button1[nbuttons].r.height = 35;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = 505; 
    button1[nbuttons].r.left = 525;
    button1[nbuttons].r.right = xres-525;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.0;
    button1[nbuttons].dcolor[1] = 0.0f;
    button1[nbuttons].dcolor[2] = 0.0f;

    //third button
    nbuttons++;
    button1[nbuttons].r.width = 100;
    button1[nbuttons].r.height = 35;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = 595; 
    button1[nbuttons].r.left = 525;
    button1[nbuttons].r.right = xres-525;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.0;
    button1[nbuttons].dcolor[1] = 0.0f;
    button1[nbuttons].dcolor[2] = 0.0f;

    //fourth button
    nbuttons++;
    button1[nbuttons].r.width = 100;
    button1[nbuttons].r.height = 35;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = 685; 
    button1[nbuttons].r.left = 525;
    button1[nbuttons].r.right = xres-525;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.0;
    button1[nbuttons].dcolor[1] = 0.0f;
    button1[nbuttons].dcolor[2] = 0.0f;
    
    //fifth  button
    nbuttons++;
    button1[nbuttons].r.width = 100;
    button1[nbuttons].r.height = 35;
    button1[nbuttons].r.centerx = (float)xres/2.0;
    button1[nbuttons].r.centery = 775; 
    button1[nbuttons].r.left =  525;
    button1[nbuttons].r.right = xres-525;
    button1[nbuttons].r.top = top1[nbuttons];
    button1[nbuttons].r.bot = bottom1[nbuttons];
    button1[nbuttons].click = 0;
    button1[nbuttons].over = 0;
    button1[nbuttons].down = 0;
    button1[nbuttons].dcolor[0] = 0.0f+0.0;
    button1[nbuttons].dcolor[1] = 0.0f;
    button1[nbuttons].dcolor[2] = 0.0f;
}

void Analy_show_menu()
{
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
	bubble[nbubbles].radius = rnd() * 12.0 + 1.0;
	bubble[nbubbles].color[0] = 1.0;
	bubble[nbubbles].color[1] = 1.0;
	bubble[nbubbles].color[2] = 1.0;
	++nbubbles;
    }

    //move bubbles
    for (int i=0; i<nbubbles; i++) {
	bubble[i].force[0] = rnd() * 0.5 - 0.25;
	bubble[i].force[1] = 0.0000000000001 + bubble[i].radius*0.003;
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

    //	glColor3i(9, 60, 235);
    for(int i=0; i<5; i++) {
	if (button1[i].over) {
	    glLineWidth(2);
	    glBegin(GL_LINE_LOOP);
	    glVertex2i(button1[i].r.left-2, button1[i].r.bot-2);
	    glVertex2i(button1[i].r.left-2, button1[i].r.top+2);
	    glVertex2i(button1[i].r.right+2, button1[i].r.top+2);
	    glVertex2i(button1[i].r.right+2, button1[i].r.bot-2);
	    glVertex2i(button1[i].r.left-2, button1[i].r.bot-2);
	    glVertex2i(button1[i].r.left-2, button1[i].r.top+2);
	    glEnd();
	    glLineWidth(1);
	    glColor3fv(button1[i].dcolor);
	} else {
	    glColor3ub(9, 60, 235);
	}

	int offset1=90;
	int offsetx1=500;
	int offsety1=200;
	//glColor3i(9, 60, 235);
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
	//vertices for box 5: 	(725, 90)
    	//			(725, 160)
    	//			(525, 160)
    	//			(525, 90)
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBegin(GL_QUADS);
	glVertex2i((xres-offsetx1-25),((offsety1)*3)- i*offset1-150);
	glVertex2i((xres-offsetx1-25),(yres-offsety1-30)-i*offset1-150);
	glVertex2i((offsetx1+25),(yres-offsety1-30)-i*offset1-150);
	glVertex2i((offsetx1+25),((offsety1)*3)-i*offset1-150);
	glEnd();
	glPopMatrix();
    }

    Rect r;

    //menu boxes names are created here
    //so each one has a position to match to be
    //centered in each box
    r.bot = 475; 
    r.left =605 ;
    r.center = 0; 
    ggprint16(&r, 0, 0x00ffffff, "Start");

    r.bot = 385;
    r.left =605 ;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Help");

    r.bot = 295; 
    r.left =595;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Settings");

    r.bot = 205; 
    r.left =580 ;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "High Score");
    
    r.bot = 115; 
    r.left =607 ;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Quit");

    r.bot = 30;
    r.left = 250;
    r.center = 0;
    ggprint12(&r, 12, 0x00fffffff, "Created by: Ana Butanda,"\
	    " Mark Felisilda, Esteban Lopez, Analy Velazquez -"\
	    "CMPS 3350 [Software Engineering] Spring 2017");
}

void Analy_ev(XEvent *e)
{
    static int savex=0;
    static int savey=0;
    int x;
    int y;
    int lbutton=0;
    if(lbutton) {};
    int rbutton=0;
    if(e->type == ButtonRelease) {
	return;
    }
    if(e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    lbutton =1;
	}
	if (e->xbutton.button==3) {
	    rbutton = 1;
	    if(rbutton) {}
	}
    } 

    x = e->xbutton.x;
    y = e->xbutton.y;

    if(savex != e->xbutton.x || savey != e->xbutton.y) {
	savex = e->xbutton.x;
	savey = e->xbutton.y;
    }

    int nbuttons = 5;
    for (int i=0; i<nbuttons; i++) {
	button1[i].over=0;
	if (x >= button1[i].r.centerx - button1[i].r.width &&
		x<= button1[i].r.centerx + button1[i].r.width &&
		y>= button1[i].r.centery - button1[i].r.height &&
		y <= button1[i].r.centery + button1[i].r.height) {
	    button1[i].over=1;
	    if (button1[i].over) {
		if(lbutton) {
		    switch (i) {
			case 0:
			    state_menu = 0;
			    break;

			case 1:
			    state_help =1;
			    break;

			case 2:
			    state_charsel = 1;
			    break;
			    
			case 3:

			    break;

			case 4:
			    exit(0);
			    
		    }
		}
	    }
	}
    }
}


void Analy_show_end() 
{

    glColor3ub(0, 204, 204);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(0, yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
 
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

    Rect r;

    r.bot = 800; 
    r.left = 585 ;
    r.center = 0; 
    ggprint16(&r, 0, 0x00ffffff, "GAME OVER");


    for(int i=0; i<4; i++) {
    r.bot = 550; 
    r.left =100 ;
    r.center = 0; 
    ggprint16(&r, 0, 0x00ffffff, "Help Menu Developer: Ana Butanda");
    
    r.bot = 530; 
    r.left =100 ;
    r.center = 0; 
    ggprint16(&r, 0, 0x00ffffff, "\"Life is better when you're running it\"");

    r.bot = 510; 
    r.left =100 ;
    r.center = 0; 
    ggprint16(&r, 0, 0x00ffffff, "         -Ana Butanda");
    
    r.bot = 450;
    r.left =100 ;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Lead Graphics Design: Mark Felisilda");

    r.bot =430;
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "\"We must embrace pain and");

    r.bot =410;
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "burn it as fuel for our journey\"");


    r.bot =390;
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "         -Sonic");

    r.bot = 350; 
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Physics Monkey: Esteban Lopez");

    r.bot = 330; 
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "\"If the grass is greener on the other side,");

    
    r.bot = 310; 
    r.left =100;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "you still have to mow it\"");

    r.bot = 250; 
    r.left =100 ;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Menu Creator: Analy Velazquez");  
    
    r.bot= 230;
    r.left =100;
    r.center=0;
    ggprint16(&r, 0, 0x00fffffff, "\"The difference between school and life?");
    
    r.bot= 210;
    r.left =100;
    r.center=0;
    ggprint16(&r, 0, 0x00ffffff, "In school, you're taught a lesson and then given a test.");
   
    r.bot= 190;
    r.left =100;
    r.center=0;
    ggprint16(&r, 0, 0x00ffffff, "In life, you're given a test that teaches you a lesson.\"");
    
    r.bot= 170;
    r.left =100;
    r.center=0;
    ggprint16(&r, 0, 0x00ffffff,  "         -Tom Bodett" );
    }
}

