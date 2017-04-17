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

#define MAX_BLOCKS 1000
#define PIXEL_WIDTH 64

int total_boxes = 0;
int num_blocks_wide = 0;
//extern int xres;
//extern int yres;
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

//struct Vec{
//	float x, y, z;
//};

struct Shape {
	float width, height;
	float radius;
	float x,y,z;
	Vec center;
	Shape(){
		width = PIXEL_WIDTH;
		height = PIXEL_WIDTH;
	}
};

Shape platform_boxes[MAX_BLOCKS];

using namespace std;

void print_esteban()
{	
	string esteban = "Esteban";
	cout << esteban << endl;
}

void render_floor()
{
	float w, h;
	//try to make a brick size a factor of the pixel size
	//32 is the size
	float pixels = 64;
	//eventually I want this to read from a text file
	//this is just a test first
	//glClear(GL_COLOR_BUFFER_BIT);
	w=pixels;
	h=pixels;
	//probably should count the number of boxes as I fill the array
	//for now just trying to get collision detections bw octopus
	//and world floor
	num_blocks_wide = xres/50;
	//int arr[num_blocks_wide];
	int arr[MAX_BLOCKS];
	float pix_center = PIXEL_WIDTH/2;

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
	//int total_boxes = 0;
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
		total_boxes += number; 
		i++;	
	}
	num_blocks_wide = i;
	ifs.close();
	//for (int i = 0; i < 25; i++)
	//cout << "INDEX " << i << " = " << arr[i] << endl;

	//read_by_char(filename);
	int box_num=0;
	for (int i = 0; i < num_blocks_wide; i++) {
		platform_boxes[box_num].x = pix_center + (i*PIXEL_WIDTH);
		//glPushMatrix();
		//coordinate with Mark to see how he wants to put image
		//tiles in the world
		//also coordinate with Adam to work out collision detection
		/*if (i%2 == 0) 
		  glColor3ub(255,0,0);
		  else 
		  glColor3ub(0,255,0);*/
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		//int j = 0;
		float position;
		//#ifdef RTH
		for (int j = 0 ; j < arr[i]; j++) {
			//Need this variable for K&R line length
			position = pix_center + (j*PIXEL_WIDTH) ;
			platform_boxes[box_num].y = position; 
			//#endif //RTH
			glPushMatrix();
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex2i(w*i,h*j);
			glTexCoord2f(0.0, 0.0); 
			glVertex2i(w*i,h*j+pixels);
			glTexCoord2f(1.0, 0.0);
			glVertex2i(w*i+pixels,h*j+pixels);
			glTexCoord2f(1.0, 1.0);
			glVertex2i(w*i+pixels,h*j);
			glEnd();
			glPopMatrix();
			//#ifdef RTH
			box_num++;
		}
		//#endif //RTH
	}
	}

	//checks collision between octopus and platforms
	void el_platform_collision(Game *g)
	{
		//if (g){};
		for (int i = 0 ; i < total_boxes; i++) {
			if(g->ship.pos[0] <= platform_boxes[i].x +
					platform_boxes[i].width && 
					g->ship.pos[0] >= platform_boxes[i].x -
					platform_boxes[i].width &&
					g->ship.pos[1] < platform_boxes[i].y +
					platform_boxes[1].height &&
					g->ship.pos[1] > platform_boxes[i].y -	
					platform_boxes[1].height) {
			/*	g->ship.pos[0] = platform_boxes[i].x +
					platform_boxes[i].width;
				g->ship.pos[1] = platform_boxes[i].y +
					platform_boxes[i].height*/;
			}
		}	
	}

	//float el_gravity(Game *g){
	float el_gravity()
	{
		return octogravity;
	}

	void el_gravity_f(Game *g)
	{
		//if (g){};
		cout<<"DEBUG: nasteroids = "<< g->nasteroids << endl;
		g->ship.vel[1] += octogravity;
	}

	//void read_by_char(string filename)
