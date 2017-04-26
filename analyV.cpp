//Analy Velazquez
//Armed and Ready

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <cstdlib> // 
#include <unistd.h> //
#include <ctime> //
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
#define random(b) (rand()%b)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
const double physicsRate = 1.0 / 60.0;
/*
 *Bubbles 
struct Bubbles {
    Vec pos;
    Vec vel;
    int nverts;
    Flt radius;
    Vec vert[8];
    float angle;
    float rotate;
    float color[3];
    struct Bubbles *prev;
    struct Bubbles *next;
    Bubbles() {
	prev = NULL;
	next = NULL;
    }
};

struct Bgame{
    int nbubbles;
    Bubbles *ahead;
    Bgame () {
	ahead = NULL;
    }
};
void init(Bgame *g);
*/

void physics(void);


void print_Analy() 
{
    string analy= "analy";
    cout << analy << endl;
}

int showRain=0;
typedef struct t_raindrop {
    int type;
    int linewidth;
    int sound;
    Vec pos;
    Vec lastpos;
    Vec vel;
    Vec maxvel;
    Vec force;
    float length;
    float color[4];
    struct t_raindrop *prev;
    struct t_raindrop *next;
} Raindrop;
Raindrop *ihead=NULL;
int ndrops=1;
int totrain=0;
int maxrain=0;
void deleteRain(Raindrop *node);
void cleanupRaindrops(void);
void drawRaindrops(void);

void Analy_show_menu()
{
    Rect r;
    
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
     

    //glColor3ub(0, 204, 204);
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
 
/*glTexCoord2f(0.0, 1.0); glVertex2i(0, 0);
    glTexCoord2f(0.0, 0.0); glVertex2i(0, yres);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres, yres);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres, 0);
   */
 glEnd();
    glPopMatrix();

    //glColor3ub(0, 204, 204);
    glBindTexture(GL_TEXTURE_2D, name_texture);
    glPushMatrix();
    glBegin(GL_QUADS);
    /*glTexCoord2f(0.0, 1.0); glVertex2i(190, 385);
    glTexCoord2f(0.0, 0.0); glVertex2i(190, yres-385);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres-190, yres-385);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres-190, 385);*/
    glTexCoord2f(0.0, 0.0); glVertex2i(190, yres-385);
    glTexCoord2f(1.0, 0.0); glVertex2i(xres-190, yres-385);
    glTexCoord2f(1.0, 1.0); glVertex2i(xres-190, 385);
    glTexCoord2f(0.0, 1.0); glVertex2i(190, 385);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    if (showRain)
	drawRaindrops();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
glBindTexture(GL_TEXTURE_2D, 0);
    drawRaindrops();

    r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint16(&r, 16, 0x00ffffff, "Armed & Ready");
    ggprint16(&r, 16, 0x00ffffff, "Main Menu");
    ggprint16(&r, 16, 0x00ffffff, "Play");
    ggprint16(&r, 16, 0x00ff0000, "Help");
    ggprint16(&r, 16, 0x00ff0000, "Rules");
    ggprint8b(&r, 16, 0x00ff0000, "Press S to start");

    const int n=0;
    int i;
    for (i=0; i<n; i++) {
	Raindrop *node = (Raindrop *)malloc(sizeof(Raindrop));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float)yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->color[0] = rnd() * 0.2f + 0.8f;
	node->color[1] = rnd() * 0.2f + 0.8f;
	node->color[2] = rnd() * 0.2f + 0.8f;
	node->color[3] = rnd() * 0.5f + 0.3f; //alpha
	node->linewidth = random(8)+1;	//larger linewidth = faster speed
	    node->maxvel[1] = (float)(node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f + rnd();
	//put raindrop into linked list
	    node->next = ihead;
	if (ihead != NULL)
	    ihead->prev = node;
	ihead = node;
	++totrain;
    }


    Raindrop *node = ihead;
    while (node) {
	glPushMatrix();
	glTranslated(node->pos[0],node->pos[1],node->pos[2]);
	glColor4fv(node->color);
	glLineWidth(node->linewidth);
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, node->length);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);


} //deleted if umcommenting bubbles
void cleanupRaindrops(void)
{
    Raindrop *s;
    while (ihead) {
	s = ihead->next;
	free(ihead);
	ihead = s;
    }
    ihead=NULL;
}

void deleteRain(Raindrop *node)
{
    //remove a node from linked list
    //log("deleteRain()...\n");
    if (node->prev == NULL) {
	if (node->next == NULL) {
	    //Log("only 1 item in list.\n");
	    ihead = NULL;
	} else {
	    //Log("at beginning of list.\n");
	    node->next->prev = NULL;
	    ihead = node->next;
	}
    } else {
	if (node->next == NULL) {
	    //Log("at end of list.\n");
	    node->prev->next = NULL;
	} else {
	    //Log("in middle of list.\n");
	    node->prev->next = node->next;
	    node->next->prev = node->prev;
	}
    }
    free(node);
    node = NULL;
}


void createRaindrop(const int n)
{
    //create new rain drops...
    int i;
    for (i=0; i<n; i++) {
	Raindrop *node = (Raindrop *)malloc(sizeof(Raindrop));
	if (node == NULL) {
	    Log("error allocating node.\n");
	    exit(EXIT_FAILURE);
	}
	node->prev = NULL;
	node->next = NULL;
	node->sound=0;
	node->pos[0] = rnd() * (float)xres;
	node->pos[1] = rnd() * 100.0f + (float)yres;
	VecCopy(node->pos, node->lastpos);
	node->vel[0] = node->vel[1] = 0.0f;
	node->color[0] = rnd() * 0.2f + 0.8f;
	node->color[1] = rnd() * 0.2f + 0.8f;
	node->color[2] = rnd() * 0.2f + 0.8f;
	node->color[3] = rnd() * 0.5f + 0.3f; //alpha
	node->linewidth = random(8)+1;	//larger linewidth = faster speed
	    node->maxvel[1] = (float)(node->linewidth*16);
	node->length = node->maxvel[1] * 0.2f + rnd();
	//put raindrop into linked list
	    node->next = ihead;
	if (ihead != NULL)
	    ihead->prev = node;
	ihead = node;
	++totrain;
    }
}

void checkRaindrops()
{
    if (random(100) < 50) {
	createRaindrop(160);
    }

    Raindrop *node = ihead;
    while (node) {

	node->vel[1] += gravity;
	VecCopy(node->pos, node->lastpos);
	node->pos[0] += node->vel[0] * timeslice;
	node->pos[1] += node->vel[1] * timeslice;
	if (fabs(node->vel[1]) > node->maxvel[1])
	    node->vel[1] *= 0.96;
	node->vel[0] *= 0.999;


	node = node->next;
    }

    int n=0;
    node = ihead;

    if (node->pos[1] < -20.0f) {
	Raindrop *savenode = node->next;
	deleteRain(node);
	node = savenode;
//	continue;
    }
    node = node->next;

if (maxrain < n)
    maxrain = n;
}

void physics(void)
{
   if (showRain)
	checkRaindrops();
}


void drawRaindrops(void)
{	
    Raindrop *node = ihead;
    while (node) {
	glPushMatrix();
	glTranslated(node->pos[0],node->pos[1],node->pos[2]);
	glColor4fv(node->color);
	glLineWidth(node->linewidth);
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, node->length);
	glEnd();
	glPopMatrix();
	node = node->next;
    }
    glLineWidth(1);
}



/*
	Bubbles *b = new Bubbles;
//	b->nverts=100;
    for (int i=0; i<15; i++) {
//	Bubbles *b =new Bubbles;
	b->nverts=8;
	//b->radius = 8.0f + 4.0f;
//	b->nverts=100;
	b->radius = 100;
//	Flt r2 = b->radius / 2.0f;
	Flt angle = 0.0f;    
	Flt inc = (PI * 2.0f) / (Flt)b->nverts;
	for (int j=0; j<b->nverts; j++) {
	    b->vert[j][0] = cos(angle) * ( b->radius);
	    b->vert[j][1] = sin(angle) * ( b->radius);
	    angle += inc;
	}

	b->pos[0] = (Flt)(rand() % xres);
	b->pos[1] = (Flt)(rand() % yres);
	b->pos[2] = 0.0f;
	b->angle = 0.0f;
	b->rotate = rnd() * 4.0f - 2.0f;
	b->color[0] = 0.8f;
	b->color[1] = 0.8f;
	b->color[2] = 0.7f;
	b->vel[0] = rnd() * 0.5 - 0.25; 
	b->vel[1] = rnd() * 0.5 - 0.25;
	
	//std::cout << "bubbles" << std::endl;
}
   
   // Bubbles *b = g->ahead;
    //b=g->ahead;
    while(b) {
	glColor3fv(b->color); 
	glPushMatrix();
	glTranslatef(b->pos[0], b->pos[1], b->pos[2]);
	glRotatef(b->angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int j=0; j<b->nverts; j++) {
	    glVertex2f(b->vert[j][0], b->vert[j][1]);
	}
	glEnd();

	glPopMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(b->pos[0], b->pos[1]);
	glEnd();
	b=b->next;
 
    }
    }
*/

