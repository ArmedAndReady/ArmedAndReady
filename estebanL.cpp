//author: Esteban Lopez
//date: Spring 2017
//purpose: 

#include <iostream>
#include <cstring>
#include <GL/glx.h>

extern int xres;
extern int yres;

using namespace std;

void print_esteban()
{	
	string esteban = "Esteban";
	cout << esteban << endl;
}

void render_floor()
{
	float w, h;
	//eventually I want this to read from a text file
	//this is just a test first
	//glClear(GL_COLOR_BUFFER_BIT);
	w=50;
	h=50;
	int num_blocks_wide = xres/50;
	int num_blocks_high = yres/50;

	for (int i = 0; i < num_blocks_wide; i++) {
		glPushMatrix();
		//coordinate with Mark to see how he wants to put image
		//tiles in the world
		if (i%2 == 0)
			glColor3ub(255,0,0);
		else 
			glColor3ub(0,255,0);
		glBegin(GL_QUADS);
		glVertex2i(w*i,0);
		glVertex2i(w*i,50);
		glVertex2i(w*i+50,50);
		glVertex2i(w*i+50,0);
		glEnd();
		glPopMatrix();
	}
}

