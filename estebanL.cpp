//author: Esteban Lopez
//date: Spring 2017
//purpose: Work on group project 

#include <iostream>
#include <cstring>
#include <GL/glx.h>
#include <cstdlib> /*srand, rand*/
#include <ctime>
#include <fstream>
#include "ppm.h"
#include "global.h"


extern int xres;
extern int yres;
extern Ppmimage *floorImage;
extern GLuint floorTexture; 
//extern const float octogravity;
//extern struct game->ship.vec;
//extern struct Game game;
//extern Game game;
//extern int game->nasteroids;
const float octogravity = -0.0006f;
int floor_set = 0;

//void read_by_char(string);

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
	//int num_blocks_high = yres/50;
	//max_block_height may be too high depending on physics of octopus
	//int max_block_height = num_blocks_high/3;

	int arr[num_blocks_wide];

	//idea: use sea stories like moby dick and 10,000 leagues under the sea
	//to create world blocks after doing a frequency analysis
	//this if statement causes game to freeze somehow
	//#define RTH //Random Tile Height
#ifdef RTH
	//trying to fill in array once
	//this destroys the program, must learn to use a text file
	if (!floor_set) {
		for (int i = 0; i < num_blocks_wide ; i++) {
			arr[i] = rand() % max_block_height + 1; 
		}
		floor_set = 1;
	}
#endif //RT
	string filename = "level1.txt";
	ifstream ifs;
	int number;
	//char c;
	int i = 0;
	ifs.open(filename.c_str());
	//while (!ifs.eof()) {
	while (ifs >> number && !ifs.eof()) {
	//for (int i = 0; i < 25; i++) {
		//ifs.get(c);
		//arr[i] << c;
		//c >> arr[i];
		arr[i] = number; 
		i++;	
	}
	ifs.close();
	//for (int i = 0; i < 25; i++)
		//cout << "INDEX " << i << " = " << arr[i] << endl;

	//read_by_char(filename);

	for (int i = 0; i < num_blocks_wide; i++) {
		//glPushMatrix();
		//coordinate with Mark to see how he wants to put image
		//tiles in the world
		//also coordinate with Adam to work out collision detection
		/*if (i%2 == 0) 
			glColor3ub(255,0,0);
		else 
			glColor3ub(0,255,0);*/
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		int j = 0;
		//#ifdef RTH
		for (j = 0 ; j < arr[i]; j++) {
			//#endif //RTH
			glPushMatrix();
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0); glVertex2i(w*i,h*j);
			glTexCoord2f(0.0, 0.0); glVertex2i(w*i,h*j+50);
			glTexCoord2f(1.0, 0.0); glVertex2i(w*i+50,h*j+50);
			glTexCoord2f(1.0, 1.0); glVertex2i(w*i+50,h*j);
			glEnd();
			glPopMatrix();
			//#ifdef RTH
		}
		//#endif //RTH
	}
}

//float el_gravity(Game *g){
float el_gravity(){
	return octogravity;
}

void el_gravity_f(Game *g){
	if (g){};
	//cout<<"DEBUG: nasteroids = "<< g->nasteroids << endl;
	//g->ship.vel[1] += octogravity;
	//g->ship.vel[1] += octogravity;
}

//void read_by_char(string filename)
