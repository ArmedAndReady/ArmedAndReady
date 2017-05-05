// Author:Ana Butanda 
// Date: Spring 2017
// Purpose: Help screen for Armed and Ready 
// The buttons highlight when over 
// and when clicked the browser opens to specific page
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <GL/glx.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>

extern"C"{
#include "fonts.h"
}

extern int xres;
extern int yres;
void ab_init();
extern int state_help;

using namespace std;

void print_Ana ()
{
    string ana ="ana" ;
    cout << ana << endl;
}

struct Vec
{
    float x,y,z;
};

typedef struct t_button 
{
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
    //float x;
    //float y;

} Button;
Button button[4];

#ifdef USE_OPENAL_SOUND
ALuint alBufferDrip, alBufferTick;
ALuint alSourceDrip, alSourceTick;
#endif


struct Global 
{
    int done;
    int nbox;
    Global (){
	done = 0;
	nbox = 0;
    }
} g;

int offset =105;
int offsetx =150;
int offsety = 200;

void ab_init()
{
    int offsets[4];
    int top[4];
    int bottom[4];
    for (int i=0;i<4;i++){
	bottom[i]= offsety*3- i*offset;
	top[i]=yres-offsety-i*offset;
	offsets[i]=(top[i]+bottom[i])/2;
    }
    int nbuttons=0;
    //button1	
    button[nbuttons].r.width=475;
    button[nbuttons].r.height=50;
    button[nbuttons].r.centerx =(float)xres/2.0;
    button[nbuttons].r.centery =200; 
    button[nbuttons].r.left = 150;
    button[nbuttons].r.right = xres-150;
    button[nbuttons].r.top = top[nbuttons];
    button[nbuttons].r.bot = bottom[nbuttons];
    button[nbuttons].click=0;
    button[nbuttons].over=0;
    button[nbuttons].down=0;
    button[nbuttons].dcolor[0]=0.2f+0.5;
    button[nbuttons].dcolor[1]=0.4f;
    button[nbuttons].dcolor[2]=0.7f;
    //button 2

    nbuttons++;
    button[nbuttons].r.width=475;
    button[nbuttons].r.height=50;
    button[nbuttons].r.centerx =(float)xres/2.0;
    button[nbuttons].r.centery =offsets[3];
    button[nbuttons].r.left = 150;
    button[nbuttons].r.right = xres-150;
    button[nbuttons].r.top = top[nbuttons];
    button[nbuttons].r.bot = bottom[nbuttons];
    button[nbuttons].click=0;
    button[nbuttons].over=0;
    button[nbuttons].down=0;
    button[nbuttons].dcolor[0]=0.2f+0.5;
    button[nbuttons].dcolor[1]=0.4f;
    button[nbuttons].dcolor[2]=0.7f;
    //button 3

    nbuttons++;
    button[nbuttons].r.width=475;
    button[nbuttons].r.height=30;
    button[nbuttons].r.centerx =(float)xres/2.0;
    button[nbuttons].r.centery =offsets[nbuttons];
    button[nbuttons].r.left = 150;
    button[nbuttons].r.right = xres-150;
    button[nbuttons].r.top = top[nbuttons];
    button[nbuttons].r.bot = bottom[nbuttons];
    button[nbuttons].click=0;
    button[nbuttons].over=0;
    button[nbuttons].down=0;
    button[nbuttons].dcolor[0]=0.2f+0.5;
    button[nbuttons].dcolor[1]=0.4f;
    button[nbuttons].dcolor[2]=0.7f;
    //button 4

    nbuttons++;
    button[nbuttons].r.width=475;
    button[nbuttons].r.height=20;
    button[nbuttons].r.centerx =(float)xres/2.0;
    button[nbuttons].r.centery =offsets[1];
    button[nbuttons].r.left = 150;
    button[nbuttons].r.right = xres-150;
    button[nbuttons].r.top = top[nbuttons];
    button[nbuttons].r.bot = bottom[nbuttons];
    button[nbuttons].click=0;
    button[nbuttons].over=0;
    button[nbuttons].down=0;
    button[nbuttons].dcolor[0]=0.2f+0.5;
    button[nbuttons].dcolor[1]=0.4f;
    button[nbuttons].dcolor[2]=0.7f;
}

void ana_show_help () 
{
    Rect r;
    int x =xres;
    int y = yres;
    if (x||y) {};
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_QUADS);
    glVertex2i((xres-100),(100));
    glVertex2i((xres-100),(yres-100));
    glVertex2i((100),(yres-100));
    glVertex2i((100),(100));
    glEnd();
    r.bot = 750;
    r.left = 600;
    r.center= 0;
    r.center=0;
    ggprint8b(&r,16,0x0000FF, "Help Menu");
    glColor3i(255,0,0);
    for (int i=0;i<4;i++){

	if(button[i].over){
	    glLineWidth(2);
	    glBegin(GL_LINE_LOOP);
	    glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
	    glVertex2i(button[i].r.left-2,  button[i].r.top+2);
	    glVertex2i(button[i].r.right+2, button[i].r.top+2);
	    glVertex2i(button[i].r.right+2, button[i].r.bot-2);
	    glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
	    glEnd();
	    glLineWidth(1);
	    glColor3fv(button[i].dcolor);
	}
	else{		
	    glColor3f(0.0,1.0/(2*i),2.0);
	}
	glBegin(GL_QUADS);
	glVertex2i((xres-offsetx),((offsety)*3)- i*offset);

	glVertex2i((xres-offsetx),(yres-offsety)-i*offset);
	glVertex2i((offsetx),(yres-offsety)-i*offset);
	glVertex2i((offsetx),((offsety)*3)-i*offset);
	glEnd();

    }
    r.bot= button[0].r.centery+450;
    r.left =button[0].r.centerx;
    ggprint12(&r,0,0x00000000,"Controls");
    r.bot= button[1].r.centery;
    r.left = button[1].r.centerx;
    ggprint12(&r,0,0x00000000,"Back");
    r.bot= button[2].r.centery;
    r.left = button[2].r.centerx;
    ggprint12(&r,0,0x00000000,"About Us");
    r.bot= button[3].r.centery;
    r.left =button[3].r.centerx;
    ggprint12(&r,0,0x00000000,"How To Play");

}

void abcm(XEvent *e)
{
    cout<<"in abcm"<<endl;
    static int savex = 0;
    static int savey = 0;
    int x,y;
    int lbutton=0;
    if (lbutton) {};
    int rbutton=0;
    if (e->type == ButtonRelease){
	cout<<"abcm 1"<<endl;

	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    lbutton = 1;
	    cout<<"abcm 2"<<endl;
	}
	if (e->xbutton.button==3) {
	    cout<<"abcm 3"<<endl;
	    rbutton=1;
	    if (rbutton) {}
	}
    }
    cout <<"abcm 4"<<endl;
    x = e->xbutton.x;
    y = e->xbutton.y;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
	savex = e->xbutton.x;
	savey = e->xbutton.y;

	cout <<"abcm 5"<<endl;
    }

    cout <<"abcm 6"<<endl;
    int nbuttons= 4;
    for (int i=0; i<nbuttons; i++) {
	button[i].over = 0;
	cout<<"for loop"<<endl;
	if (x >= button[i].r.centerx - button[i].r.width &&
		x <= button[i].r.centerx + button[i].r.width &&
		y >= button[i].r.centery-button[i].r.height &&
		y <= button[i].r.centery+ button[i].r.height){
	    button[i].over=1;
	    cout<<"if loop"<<i<<endl;
	    if (button[i].over) {
		if (lbutton) {
		    switch(i) {
			case 0:
			    cout << "Controls"<<endl;
			    system("firefox http://www.cs.csub.edu/~abutanda/3350/Controls/contols.html");
			    break;
			case 1:
			    cout << "How to play"<<endl;
			    system("firefox http://www.cs.csub.edu/~abutanda/3350/HowToPlay/howtoplay.html");
			    break;
			case 2:
			    cout << "About"<<endl;
			    system("firefox https://www.cs.csub.edu/~abutanda/3350/home/home.html");
			    break;
			case 3:
			    cout << "back"<<endl;
			    state_help = 0;
			    break;
		    }
		}
	    }
	}
    }
}




