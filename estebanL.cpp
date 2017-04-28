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
#define PIXEL_WIDTH 128
//#define PIXEL_WIDTH 180
#define MAX_BLOCKS_WIDE 200

//for different unit tests and implementations
//#define FLOOR_STUDY


int total_boxes = 0;
int num_blocks_wide = 0;
extern int xres;
extern int yres;
extern Ppmimage *floorImage;
extern GLuint floorTexture; 
//extern const float octogravity;
//extern struct game->ship.vec;
//extern struct Game game;
//extern Game game;
//extern int game->nasteroids;
const float octogravity = -0.006f;
bool collided = false;
int floor_set = 0;
int checked = 0;

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
int set_from_text[MAX_BLOCKS_WIDE];

void read_from_text();
void set_platform_boxes();
void draw_boxes();
void check_position();
void reset_platforms();
//Have Mark use if images are set and set to 1
bool image_set = 0;
float octo_position = 0.0;

using namespace std;

void print_esteban()
{	
	string esteban = "Esteban";
	cout << esteban << endl;
}


#ifdef FLOOR_STUDY
void render_floor()
{
	float w, h;
	float pixels = PIXEL_WIDTH;
	w=pixels;
	h=pixels;
	//num_blocks_wide = xres/50;
	num_blocks_wide = 10;
	int arr[MAX_BLOCKS];
	float pix_center = PIXEL_WIDTH/2;

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
	int i = 0;
	//if(!floor_set) {
	ifs.open(filename.c_str());
	total_boxes = 0;
	while (ifs >> number && !ifs.eof()) {
		arr[i] = number;
		total_boxes += number; 
		i++;	
	}
	//cout << "DEBUG: total_boxes = " << total_boxes << endl;
	num_blocks_wide = i;
	floor_set = 1;
	ifs.close();
	//}

	int box_num=0;
	for (int i = 0; i < num_blocks_wide; i++) {
		platform_boxes[box_num].x = pix_center + (i*PIXEL_WIDTH);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		float position;
		for (int j = 0 ; j < arr[i]; j++) {
			//Need this variable for K&R line length
			position = pix_center + (j*PIXEL_WIDTH) ;
			platform_boxes[box_num].y = position; 
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
			box_num++;
		}
	}
}

void el_platform_collision(Game *g)
{
	//if (g){};
	for (int i = 0 ; i < total_boxes; i++) {
		//cout << "DEBUG: i = " << i << endl;
		if(g->ship.pos[0] <= platform_boxes[i].x +
				platform_boxes[i].width && 
				g->ship.pos[0] >= platform_boxes[i].x -
				platform_boxes[i].width &&
				g->ship.pos[1] <= platform_boxes[i].y +
				platform_boxes[1].height &&
				g->ship.pos[1] >= platform_boxes[i].y -	
				platform_boxes[1].height) {
			//	cout << "DEBUG: boxes\n";
			//	g->ship.pos[0] = platform_boxes[i].x +
			//		  platform_boxes[i].width;
			g->ship.pos[1] = platform_boxes[i].y +
				platform_boxes[i].height * 1.01;
			collided =  true;	
			//g->ship.vel[1] = 0;
		}
	}	
}
#endif //FLOOR_STUDY

#ifndef FLOOR_STUDY
void render_floor(){
	read_from_text();
	set_platform_boxes();
	/*void check_position();
	  void reset_platforms();*/
 	//check_position();
	if(!image_set){
		draw_boxes();
	}
	//#define CHECK_READ_IN
#ifdef CHECK_READ_IN
	cout << "DEBUG: reading through array from text\n";
	for(int i = 0; i < num_blocks_wide; i++) {
		cout << "set_from_text["<<i<<"] = " << set_from_text[i] << endl;
	}
#endif //CHECK_READ_IN
	//void draw_boxes();
	//bool image_set = 0;
}

void read_from_text(){
	//string filename = "level1.txt";
	string filename = "test.txt";
	ifstream ifs;
	int number;
	int i = 0;
	ifs.open(filename.c_str());
	total_boxes = 0;
	while (ifs >> number && !ifs.eof()) {
		set_from_text[i] = number;
		total_boxes += number; 
		i++;	
	}
	num_blocks_wide = i;
	floor_set = 1;
	ifs.close();
}

//for reference
/*struct Shape {
  float width, height;
  float radius;
  float x,y,z;
  Vec center;
  Shape(){
  width = PIXEL_WIDTH;
  height = PIXEL_WIDTH;
  }
  };

  Shape platform_boxes[MAX_BLOCKS];*/

void set_platform_boxes()
{
	//#define CHECK_SPB
#ifdef CHECK_SPB
	cout<<"DEBUG: num_blocks_wide = " << num_blocks_wide << endl;
	cout<<"DEBUG: total_boxes = " << total_boxes << endl;
#endif //CHECK_SPB
	//think of i as width and j as height
	int num = 0;
	float w,h;
	float halved = (float)PIXEL_WIDTH/2.0;
	w = halved;
	h = halved;
	//#define CHECK_VARIABLES
#ifdef CHECK_VARIABLES
	if(!checked) {
		cout<<"DEBUG: PIXEL_WIDTH set at " << PIXEL_WIDTH << endl;
	}
#endif //CHECK_VARIABLES
	if (w || h) {};
	for (int i = 0 ; i < total_boxes; i++) {
		//platform_boxes[num].center[0]= w+(PIXEL_WIDTH*i);
		//float a = platform_boxes[num].center[0];	
		for (int j = 0; j < set_from_text[i]; j++) {
			platform_boxes[num].center[0]= w+(PIXEL_WIDTH*i);
			platform_boxes[num].center[1]=h+(PIXEL_WIDTH*j);
			float a = platform_boxes[num].center[0];	
			float b = platform_boxes[num].center[1];
			if (a||b){};
			//#define CHECK_PB_BOXES
#ifdef CHECK_PB_BOXES
			if(!checked) {
				cout<<"DEBUG: Platform num = : "<<num << endl;
				cout<<"DEBUG: Platform x = : "<<a << endl;
				cout<<"DEBUG: Platform y = : "<<b << endl;
			}
#endif //CHECK_PB_BOXES
			num++;
		}
	}
	//#define CHECK_PLATFORM_COORDINATES
#ifdef CHECK_PLATFORM_COORDINATES
	if(!checked) {	
		cout << "DEBUG: checking x and y of platforms boxes\n";
		for (int i = 0; i < total_boxes; i++) {
			cout <<"Platform box " << i+1 << " : x is " << platform_boxes[i].center[0] << ", y is " << platform_boxes[i].center[1] << endl;
		}
	}
	checked = 1;
#endif // CHECK_PLATFORM_COORDINATES
}

void draw_boxes()
{
	float w, h;
	float pixels = PIXEL_WIDTH;
	w=pixels;
	h=pixels;
	float pix_center = PIXEL_WIDTH/2;
	//int arr[MAX_BLOCKS];
	int box_num=0;
	for (int i = 0; i < num_blocks_wide; i++) {
		platform_boxes[box_num].center[0] = pix_center + (i*PIXEL_WIDTH);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		float position;
		for (int j = 0 ; j < set_from_text[i]; j++) {
			//Need this variable for K&R line length
			position = pix_center + (j*PIXEL_WIDTH) ;
			platform_boxes[box_num].center[1] = position; 
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
			box_num++;
		}
	}
}

void el_platform_collision(Game *g)
{ 
	if (g) {};
	int x_index = platform_boxes[0].center[0];
	int y_index = platform_boxes[0].center[1];
	int check_y_collision = false;
	if (y_index){}
	//#define CHECK_COORDINATES
#ifdef CHECK_COORDINATES
	if(!checked) {
		cout << "DEBUG: checking x and y of platforms boxes\n";
		for (int i = 0; i < total_boxes; i++) {
			cout <<"Platform box " << i+1 << " : x is " << platform_boxes[i].center[0] << ", y is " << platform_boxes[i].center[1] << endl;
		}
	}
	checked = 1;
#endif //CHECK_COORDINATES

	for (int i = total_boxes; i > 0; i--) {
		if (x_index == platform_boxes[i].center[0]) {
			check_y_collision = false;
			//y_index = platform_boxes[i].center[1];
		} else { 
			x_index = platform_boxes[i].center[0];
			y_index = platform_boxes[i].center[1];
			check_y_collision = true;
		}

		//cout << "DEBUG: i = " << i << endl;
		if (check_y_collision) {
			if(g->ship.pos[0] <= platform_boxes[i].center[0] +
					platform_boxes[i].width && 
					g->ship.pos[0] >= platform_boxes[i].center[0] -
					platform_boxes[i].width &&
					g->ship.pos[1] <= platform_boxes[i].center[1] +
					platform_boxes[1].height && 
					g->ship.pos[1] >= platform_boxes[i].center[1] -	
					platform_boxes[1].height) {
				//	cout << "DEBUG: boxes\n";
				//	g->ship.pos[0] = platform_boxes[i].x +
				//		  platform_boxes[i].width;
				//#define DETECT_COLLISION
#ifdef DETECT_COLLISION	
				cout <<"DEBUG: in el_platform_collision\n";
				cout <<"Collision detected with platform " << i + 1 << endl;
#endif //DETECT_COLLISION

				g->ship.pos[1] = platform_boxes[i].center[1] +
					platform_boxes[i].height * 1.01;
				collided =  true;	
				//g->ship.vel[1] = 0;
			}
		}
	}	
}
#endif //#ifndef FLOOR_STUDY

void check_position()
{
/*	if(g->ship.pos[0] > xres * 0.75){
		cout <<"TEST: Octo past 75 percent on screen\n";
	}*/
}

void reset_platforms();

float el_gravity()
{
	return octogravity;
}

void el_gravity_f(Game *g)
{
	//if (g){};
#ifdef DEBUG_GRAVITY	
	cout<<"DEBUG: nasteroids = "<< g->nasteroids << endl;
#endif //DEBUG_GRAVITY
	if (!collided) {
		g->ship.vel[1] += octogravity;
	} else { 
		g->ship.vel[1] = 0;
	}
//not a natural place for this, but use as a study
	if(g->ship.pos[0] > xres * 0.75) {
		//cout <<"TEST: Octo past 75 percent on screen\n";
		//reset_platforms(); move both octopus and platforms back and introduce new ones.
	}
}

void el_jump(Game *g)
{   
	//seems more like a swim up rather than a jump
	//might be ideal for this game 	
	g->ship.vel[1] -= octogravity * 3;
	collided = false;
}
//void read_by_char(string filename)
