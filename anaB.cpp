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
    Rect r;
    float width, height;
    float radius;
    int click;
    int down;
    int over;
    Vec center;
};
Shape button [4];
struct Global {
    int xres;
    int yres;
    int done;
    Shape box;
    //Shape button [4];
    int nbox;
    //
#ifdef USE_OPENAL_SOUND
    ALuint alBufferDrip, alBufferTick;
    ALuint alSourceDrip, alSourceTick;
#endif
    Global() {
	xres = 800;
	yres = 600;
	done = 0;
	nbox = 0;
    }
} g;



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
	box[i].click=0;
	box[i].over=0;
    }
    int offset =105;
    int offsetx =150;
    int offsety = 200;
    for (int i=0;i<4;i++){
	glColor3f(0.0,1.0/(2*i),2.0);
	glBegin(GL_QUADS);
	glVertex2i((xres-offsetx),((offsety)*3)- i*offset);
	glVertex2i((xres-offsetx),(yres-offsety)-i*offset);
	glVertex2i((offsetx),(yres-offsety)-i*offset);
	glVertex2i((offsetx),((offsety)*3)-i*offset);	    
	glEnd();

    }

    r.bot= box[0].center.y+190;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000000,"Controls");
    r.bot= box[0].center.y+90;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000000,"How to play");
    r.bot= box[0].center.y-15;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000000,"Release Notes");
    r.bot= box[0].center.y-120;
    r.left =box[0].center. x;
    ggprint12(&r,0,0x00000000,"About");
    //r.bot= box[0].center.y-120;
    //r.leffil://github.com/ArmedAndReady/ArmedAndReadyt=box[0].center.x;
    //ggprint12(&r,0,0x00000000,"About");
}

//#ifdef BUTTONS
void mouse_click(int ibutton, int action, int x, int y)
{
    //int con;
    if(ibutton){};
    if(action){};
    if(x){};
    if(y){};
    int nbuttons =4;
    if (action == 1) {
	//	int i,j,k,m=0;

	//center of a grid
	//	int cent[2];
	//buttons?                                                         
	for (int i=0; i<nbuttons; i++) {
	    if (button[i].over) {                                                           
		button[i].down = 1;
		button[i].click = 1;                         
	    if (i==0) {
		   // system();
			//user clicked QUIT		                                                  
			//  done = 1;
		}
	    }
	    if (i==1) {
		//system();
		//user clicked button 0
		//reset_grids();
	    }

	    if (i==2) {
		//system();
		//user clicked button 0
		//reset_grids();
	    }
	    if (i==3) {
		//sysytem();
		//user clicked button 0
		//reset_grids();
	    }
	}
    }
}

void check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;
    //Log("check_mouse()...\n");
    int x,y;
    //int i,j,k;

    //int cent[2];
    int lbutton=0;
    int rbutton=0;
    if (e->type == ButtonRelease) {
	mouse_click(e->xbutton.button, 2, e->xbutton.x, e->xbutton.y);
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1){                                                                   
	    //Left button is down    
	    lbutton=1;                                                                      }
	if (e->xbutton.button==3) {                    
	    //Right button is down
	    rbutton=1;
	}                           
	//Log("e->xbutton.x: %i e->xbutton.y: %i\n",e->xbutton.x,e->xbutton.y);            
	x = e->xbutton.x;
	y = e->xbutton.y;
	//reverse the y position      
	y = yres - y;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	    //Mouse moved 
	    savex = e->xbutton.x; 
	    savey = e->xbutton.y;}                
	//Log("xy: %i %i\n",x,y);     
	if (x == savex && y == savey)                                   
	    return;             
	savex=x;	
	savey=y;
	//is the mouse over any grid squares?
	//
	/*for (i=0; i<grid_dim; i++) {
	  for (j=0; j<grid_dim; j++) {
	  grid1[i][j].over=0;
	  grid2[i][j].over=0;
	  }
	  }
	  for (i=0; i<grid_dim; i++) {
	  for (j=0; j<grid_dim; j++) {
	  if (gamemode == MODE_PLACE_SHIPS) {
	  get_grid_center(1,i,j,cent);
	  if (x >= cent[0]-qsize &&
	  x <= cent[0]+qsize &&
	  y >= cent[1]-qsize &&
	  y <= cent[1]+qsize) {
	  break;
	  ://github.com/ArmedAndReady/ArmedAndRead://github.com/ArmedAndReady/ArmedAndReady
	  }
	  if (gamemode == MODE_FIND_SHIPS) {
	  get_grid_center(2,i,j,cent);
	  if (x >= cent[0]-qsize &&
	  x <= cent[0]+qsize &&
	  y >= cent[1]-qsize &&
	  y <= cent[1]+qsize) {
	  grid2[i][j].over=1;
	  break;
	  }
	  }
	  }
	  if (grid1[i][j].over) break;
	  if (grid2[i][j].over) break;
	  }*/
	//
	//is the mouse over any buttons?
	//
	//Log("check over %i buttons\n", nbuttons);
	for (int i=0; i<4; i++) {
	    button[i].over=0;
	    button[i].down=0;
	    //Log("xy: %i %i\n",x,y);
	    if (x >= button[i].r.left &&
		    x <= button[i].r.right &&
		    y >= button[i].r.bot &&
		    y <= button[i].r.top) {
		button[i].over=1;
		//Log("over button: %i\n", i);
		break;
	    }
	}
	if (lbutton)
	    mouse_click(1, 1,x,y);
	if (rbutton)
	    mouse_click(2, 1,x,y);
    }

    }
    //#endif
    //boxes and add menu options are in the game need.
    //need to link each box to a link and make it click-able.  
    //link to a webpage
    //system("picture")
    //http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php
