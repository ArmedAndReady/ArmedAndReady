//author: Esteban Lopez
//date: Spring 2017
//purpose: Work on group project 

#include <iostream>
#include <cstring>
#include <GL/glx.h>
#include <cstdlib> /*srand, rand*/
#include <ctime>

extern int xres;
extern int yres;

int floor_set = 0;

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
	//max_block_height may be too high depending on physics of octopus
	int max_block_height = num_blocks_high/3;

	int arr[num_blocks_wide];

	//idea: use sea stories like moby dick and 10,000 leagues under the sea
	//to create world blocks after doing a frequency analysis
	//this if statement causes game to freeze somehow
//#define RTH //Random Tile Height
#ifdef RTH
	if (!floor_set) {
		for (int i = 0; i < num_blocks_wide ; i++) {
			arr[i] = rand() % max_block_height +1; 
		}
		floor_set = 1;
	}
#endif //RTH

	for (int i = 0; i < num_blocks_wide; i++) {
		glPushMatrix();
		//coordinate with Mark to see how he wants to put image
		//tiles in the world
		//also coordinate with Adam to work out collision detection
		if (i%2 == 0)
			glColor3ub(255,0,0);
		else 
			glColor3ub(0,255,0);
		int j = 0;
#ifdef RTH
		for(j = 0 ; j < arr[i]; j++){
#endif //RTH
			glBegin(GL_QUADS);
			glVertex2i(w*i,h*j);
			glVertex2i(w*i,h*j+50);
			glVertex2i(w*i+50,h*j+50);
			glVertex2i(w*i+50,h*j);
			glEnd();
			glPopMatrix();
#ifdef RTH
		}
#endif //RTH
	}

}
