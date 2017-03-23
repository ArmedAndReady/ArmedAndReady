//Analy Velazquez
//Armed and Ready

#include <iostream>
#include <cstring>
#include <cstdlib> // 
#include <unistd.h> //
#include <ctime> //
#include <cmath>

#include <X11/Xlib.h>
#include <X11/keysym.h>
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

typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix [4][4];

#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(b) (rand()%b)
#define PI 3.141592653589793


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



for (int i=0; i<15; i++) {
    Bubbles *b = new Bubbles;
    b->nverts=8;
    b->radius = rnd()*80.0f + 40.0f;
    Flt r2 = b->radius / 2.0f;
    Flt angle = 0.0f;    
    Flt inc = (PI * 2.0f) / (Flt)b->nverts;
    for (int j=0; j<b->nverts; j++) {
	b->vert[j][0] = sin(angle) * (r2 + rnd() * b->radius);
	b->vert[j][1] = cos(angle) * (r2 + rnd() * b->radius);
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
    b->vel[0] = (Flt)(rnd() * 2.0f-1.0f);
    b->vel[1] = (Flt)(rnd() * 2.0f-1.0f);
    std::cout << "bubbles" << std::endl;
}


Bubbles *b = g->ahead;
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

