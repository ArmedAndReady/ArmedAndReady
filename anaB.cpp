// Author:Ana Butanda 
// Date: Spring 2017
// Purpose: Help screen for Armed and Ready 
//
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <GL/glx.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

extern"C"{
#include "fonts.h"
}
extern int xres;
extern int yres;
void ab_init();
using namespace std;
void print_Ana()
{
	string ana ="ana" ;
	cout << ana << endl;
}
/*struct Vec
	{
	float x,y,z;
	};*/
/*struct Shape
	{
	Rect r;
	float width, height;
	float radius;
	int click;
	int down;
	int over;
	Vec center;
	};
	Shape button [4];*/
typedef struct t_button {
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
//other
//Button button[MAXBUTTONS];
//int nbuttons=0;



/*struct Global {
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
} g;*/


struct Global {
	float xres;
	float yres;
	int done;
	int nbox;
	Global (){
		xres = 800;
		yres = 600;
		done = 0;
		nbox = 0;
	}
} g;

void ab_init(){
	//Button button[4];
	for (int i=0; i<4;i++){

		button[i].r.width=100;
		button[i].r.height=10;
		button[i].r.centerx =(float)xres/2.0-3.0*i;
		button[i].r.centery =yres/2.0-3.0*i;
		button[i].click=0;
		button[i].over=0;
		button[i].down=0;
	}
	//int nbuttons=0;
	/*strcpy(button[nbuttons].text, "How to Play");
		nbuttons++;
		strcpy(button[nbuttons].text, "Controls");
		nbuttons++;
	 */
}

void ana_show_help(){
	Rect r;
	int x =xres;
	int y = yres;
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
	ggprint8b(&r,16,0x00ff0000, "Help Menu");
	ggprint8b(&r, 16,0x00ff000,"Press H to toggle");
	glColor3i(255,0,0);
	int offset =105;
	int offsetx =150;
	int offsety = 200;
	for (int i=0;i<4;i++){
		if (x >= button[i].r.center - button[i].r.width &&
				x <= button[i].r.center + button[i].r.width &&
				y >= button[i].r.center-button[i].r.height &&
				y <= button[i].r.center+ button[i].r.height){
			glColor3f(1.0f, 0.0f, 0.0f);

			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
			glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
			glVertex2i(button[i].r.left-2,  button[i].r.top+2);
			glVertex2i(button[i].r.right+2, button[i].r.top+2);
			glVertex2i(button[i].r.right+2, button[i].r.bot-2);
			glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
			glEnd();
			glLineWidth(1);
		}
		glColor3f(0.0,1.0/(2*i),2.0);
		glBegin(GL_QUADS);
		glVertex2i((xres-offsetx),((offsety)*3)- i*offset);
		glVertex2i((xres-offsetx),(yres-offsety)-i*offset);
		glVertex2i((offsetx),(yres-offsety)-i*offset);
		glVertex2i((offsetx),((offsety)*3)-i*offset);
		glEnd();
		cout<<"buttons created"<<endl;
	}	 
	r.bot= button[0].r.centery+190;
	r.left =button[0].r.centerx;
	ggprint12(&r,0,0x00000000,"Controls");
	r.bot= button[0].r.centery+90;
	r.left = button[0].r.centerx;
	ggprint12(&r,0,0x00000000,"How to play");
	r.bot= button[0].r.centery-15;
	r.left = button[0].r.centerx;
	ggprint12(&r,0,0x00000000,"Release Notes");
	r.bot= button[0].r.centery-120;
	r.left =button[0].r.centerx;
	ggprint12(&r,0,0x00000000,"About");

}

void abcm(int x , int y)
{
	int i= 0;
	for (i=0; i<4; i++) {
		if (x >= button[i].r.center - button[i].r.width &&
				x <= button[i].r.center + button[i].r.width &&
				y >= button[i].r.center-button[i].r.height &&
				y <= button[i].r.center+ button[i].r.height){
			button[i].over=1;
			cout<<"over button: "<<i<<endl;
		}
	  //esteban put just for compile. get rid of later
		int lbutton = 0;
		if (button[i].over) {
			if (lbutton) {
				switch(i) {
					case 0:
						cout << "Controls";
						system("firefox http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php");
						break;
					case 1:
						cout << "How to play";
						system("firefox http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php");
						break;
					case 2:
						cout << "Release Notes";
						system("firefox http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php");
						break;
					case 3:
						cout << "About";
						system("firefox http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php");
						break;
				}
			}
		}
	}
}




//boxes and add menu options are in the game need.
//need to link each box to a link and make it click-able.  
//link to a webpage
//system("picture")
//http://www.cs.csub.edu/~abutanda/3350/help_menu/help_menu.php
