//modified by:Adam Curtis, Analy Velazquez, Ana Butanda, Mark Felicida,
//Esteban Lopez
//date:1-3-17 & turned into Inkman
//purpose:To lear how to delete linked list pointers and remove objects from
//the game
//
//Ink man shoots fron ink reserve to propel himself upward and uses tentacles
//to fling ink at targets. He gets powerups to increase potency and rapidity
//of ink shots.
//
//
//Original program: asteroids
//author:  Gordon Griesel
//date:    2014
//mod spring 2015: added constructors
//mod spring 2016: prepare for class
//mod spring 2017: modifications for lab
//
//Objectives:
//The program does not work properly because neither bullets nor asteroids
//are being deleted properly. 
//Search for the text: "Delete bullet here"
//
//Asteroids are in a doubly-linked list.
//Fix the deleteAsteroid() function.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
#include "global.h"

#define SOUND //EL trying to incorporate openAL sound
#ifdef SOUND
//EL brought in from snake.cpp
#include </usr/include/AL/alut.h>
#endif //SOUND see if sound will work

using namespace std;

extern void print_esteban();
extern void el_render_floor();
extern void el_render_background();
//extern void el_gravity(XEvent *, Game *);
extern void print_Ana();
extern void ana_show_help();
extern void ab_init();
extern void print_Mark();
extern void initCharMark();//MF
extern int mcheck_keys(XEvent *e);//MF
extern void mark_show_charsel();//MF
extern void redOctober();//MF
extern void projectileTex();//MF
extern void texGen();//MF
extern void characterSelect();
extern void print_Adam();
extern void print_Analy();
extern void Analy_show_menu();
extern void physicsB();
extern void renderB();
//int showRain=0;

extern "C" {
#include "fonts.h"
}

//defined types
#ifndef GLOBAL_H
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
			     (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
#endif //GLOBAL_H

#define COLLISION
#ifdef COLLISION
//EL
//const float octogravity = -0.0005f;

#endif //COLLISION
//AV
int state_menu = 1; 
//AB
int state_help = 0;
//MF
int state_charsel = 0;

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;

//#ifndef GLOBAL_H
//=======================================================================
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
//unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//=======================================================================
//#endif //GLOBAL_H


int xres=1250, yres=900;
#ifndef GLOBAL_H
struct Ship {
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	Ship() {
		VecZero(dir);
		pos[0] = (Flt)(xres/2);
		pos[1] = (Flt)(yres/2);
		pos[2] = 0.0f;
		VecZero(vel);
		angle = 0.0;
		color[0] = 0.0;//1.0;
		color[1] = 1.0;
		color[2] = 1.0;
	}
};

struct Bullet {
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
	struct Bullet *prev;
	struct Bullet *next;
	Bullet() {
		prev = NULL;
		next = NULL;
	}
};

struct Asteroid {
	Vec pos;
	Vec vel;
	int nverts;
	Flt radius;
	Vec vert[8];
	float angle;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
	Asteroid() {
		prev = NULL;
		next = NULL;
	}
};

const int MAX_BULLETS = 1000;

struct Game {
	Ship ship;
	Bullet *barr;
	Asteroid *ahead;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
#ifdef SOUND
	ALuint alBufferScuba;
	ALuint alSourceScuba;
#endif //SOUND
	Game() {
		ahead = NULL;
		barr = new Bullet[MAX_BULLETS];
		nasteroids = 0;
		nbullets = 0;
	}
};

//Game game;
#endif //GLOBAL_H
Game game;


//EL trying to test how image files will work with world obstacle creation
//bringing elements from rainforest to study this portion
#define IMAGE
#ifdef IMAGE
Ppmimage *floorImage=NULL;
GLuint floorTexture;

Ppmimage * parallaxImage=NULL;
GLuint parallaxTexture;

//Analy Image for menu
Ppmimage *name_image=NULL;
GLuint name_texture;

//Analy menu
Ppmimage *name_image2=NULL;
GLuint name_texture2;

#endif //IMAGE

//EL trying to mess with physics in case
//comment in ORIG_PHYSICS to revert to original game
//also comment out EL_PHYSICS
//#define ORIG_PHYSICS
#define EL_PHYSICS


int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void init(Game *g);
void init_sounds(void);
void physics(Game *game);
void render(Game *game);
#ifdef SOUND
//Error Correction add Game *g as parameter
void initSound(Game *g);
void cleanupSound(Game *g);
void playSound(ALuint source);
int sound = 0;
#endif //SOUND
//extern void el_gravity(XEvent *e, Game *g);
//extern float el_gravity(Game *g);
extern float el_gravity();
extern void el_gravity_f(Game *g);
extern void el_platform_collision(Game *g);
extern void el_jump(Game *g);
extern void el_sidescroll(Game *g, float scroll, char dir);
extern void init_el_buttons();
extern void abcm(XEvent *e);
extern int mdone;
int main(void)
{
	print_esteban();
	print_Ana();
	print_Mark();
	print_Adam();
	print_Analy();
	//For Assignment Above
	logOpen();
	initXWindows();
	init_opengl();
	//#ifdef SOUND
	//	initSound();
	//#endif //SOUND
	//EL made Game game global to be able to access from files
	//Game game;
	init(&game);
	//init_el_buttons();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
#ifdef SOUND
	initSound(&game);
#endif //SOUND
	int done=0;
	while (!done) {
		while (XPending(dpy)) {
			XEvent e;
			XNextEvent(dpy, &e);
			check_resize(&e);
			check_mouse(&e);
			abcm(&e);
			cout<<"calling abcm"<<endl;
			abcm(&e);
			done = check_keys(&e);
			mdone = mcheck_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
			physics(&game);
			physicsCountdown -= physicsRate;
		}
		render(&game);
		glXSwapBuffers(dpy, win);
	}
#ifdef SOUND
	//EL Error Correction add &game as parameter
	
	
	cleanupSound(&game);
#endif //SOUND


	cleanupXWindows();
	cleanup_fonts();
	logClose();
	return 0;
}

#ifdef SOUND//EL: see snake.cpp for reference or mistakes in this transferral 
void initSound(Game *g)
{
	alutInit(0,NULL);
	if (alGetError() != AL_NO_ERROR) {
		//printf("ERROR: alutInit()\n");
		return;
	}
	alGetError();

	float vec[6] = {0.0f, 0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);

	//EL-Error Correction: change from . to ->
	//found free scuba wav on grsites.com
	//g->alBufferScuba = alutCreateBufferFromFile("water014.wav");
	g->alBufferScuba = alutCreateBufferFromFile("underthesea.wav");
	alGenSources(1, &g->alSourceScuba);
	alSourcei(g->alSourceScuba, AL_BUFFER, g->alBufferScuba);
	alSourcef(g->alSourceScuba, AL_GAIN, 1.0f);
	alSourcef(g->alSourceScuba, AL_PITCH, 1.0f);
	//alSourcef(g->alSourceScuba, AL_LOOPING, AL_FALSE);
	//EL hunch AL_TRUE will allow AL_LOOPING to play whole sound
	alSourcef(g->alSourceScuba, AL_LOOPING, AL_TRUE);
	if (alGetError() != AL_NO_ERROR) {
		//printf("ERROR: setting source\n");
		return;
	}
}

void cleanupSound(Game *g)
{
	alDeleteSources(1, &g->alSourceScuba);
	alDeleteBuffers(1, &g->alBufferScuba);
	ALCcontext *Context = alcGetCurrentContext();
	ALCdevice *Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void playSound(ALuint source)
{
	while (sound < 9000) {
		alSourcePlay(source);
		sound++;
	}
}
#endif //SOUND

void cleanupXWindows(void)
{
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

void set_title(void)
{
	//Set the window title bar.
	XMapWindow(dpy, win);
	//EL changed from Asteroids to Armed And Ready
	XStoreName(dpy, win, "3350 - Armed And Ready");
}

void setup_screen_res(const int w, const int h)
{
	xres = w;
	yres = h;
}

void initXWindows(void)
{
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
	XSetWindowAttributes swa;
	setup_screen_res(xres, yres);
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	} 
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.event_mask = ExposureMask |
		KeyPressMask |
		KeyReleaseMask |
		ButtonPressMask |
		ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask |
		SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
			vi->depth, InputOutput, vi->visual,
			CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
	//window has been resized.
	setup_screen_res(width, height);
	//
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glOrtho(0, xres, 0, yres, -1, 1);
	set_title();
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, xres, yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, xres, 0, yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
#ifdef IMAGE
	//ppm6SaveImage("Bricks_1.png", "Bricks_1.ppm");
	//need to figure out how these work
	//ppm6SaveImage("Bricks_1.ppm", "Bricks_1.png");
	system("convert ./Game_Tiles/Sand.png ./Game_Tiles/Sand.ppm");
	floorImage = ppm6GetImage("./Game_Tiles/Sand.ppm"/*"Bricks_1.ppm"*/);
	//floorImage = ppm6GetImage("Bricks_1.png");
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, floorImage->width, floorImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorImage->data);
	unlink("./Game_Tiles/Sand.ppm");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	system("convert atlantis.jpg atlantis.ppm");
	//name_image2 = ppm6GetImage("atlantis.ppm");	
	parallaxImage = ppm6GetImage("atlantis.ppm");	
	glGenTextures(1, &parallaxTexture);
	glBindTexture(GL_TEXTURE_2D, parallaxTexture);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, parallaxImage->width, parallaxImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, parallaxImage->data);
	unlink("atlantis.ppm");


	//analy logo for menu
	glClearColor(0.0, 0.0, 0.0, 1.0);
	name_image = ppm6GetImage("logo.ppm");	
	glGenTextures(1, &name_texture);
	glBindTexture(GL_TEXTURE_2D, name_texture);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, name_image->width, name_image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, name_image->data);

	//Analy menu background
	//here we converted cool.jpg to sea1.ppm and
	//then unlink gets rid of the image once its done
	glClearColor(0.0, 0.0, 0.0, 1.0);
	system("convert cool.jpg sea1.ppm");
	name_image2 = ppm6GetImage("sea1.ppm");	
	glGenTextures(1, &name_texture2);
	glBindTexture(GL_TEXTURE_2D, name_texture2);	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, name_image2->width, name_image2->height, 0, GL_RGB, GL_UNSIGNED_BYTE, name_image2->data);
	unlink("sea1.ppm");

	texGen();




#endif //IMAGE
}

void check_resize(XEvent *e)
{
	//The ConfigureNotify is sent by the
	//server if the window is resized.
	if (e->type != ConfigureNotify)
		return;
	XConfigureEvent xce = e->xconfigure;
	if (xce.width != xres || xce.height != yres) {
		//Window size did change.
		reshape_window(xce.width, xce.height);
	}
}

void init(Game *g)
{
	//build 10 asteroids...
	for (int j=0; j<10; j++) {
		Asteroid *a = new Asteroid;
		a->nverts = 8;
		a->radius = rnd()*80.0f + 40.0f;
		Flt r2 = a->radius / 2.0f;
		Flt angle = 0.0f;
		Flt inc = (PI * 2.0f) / (Flt)a->nverts;
		for (int i=0; i<a->nverts; i++) {
			a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
			a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
			angle += inc;
		}
		a->pos[0] = (Flt)(rand() % xres);
		a->pos[1] = (Flt)(rand() % yres);
		a->pos[2] = 0.0f;
		a->angle = 0.0f;
		a->rotate = rnd() * 4.0f - 2.0f;
		a->color[0] = 0.8f;
		a->color[1] = 0.8f;
		a->color[2] = 0.7f;
		a->vel[0] = (Flt)(rnd()*2.0f-1.0f);
		a->vel[1] = (Flt)(rnd()*2.0f-1.0f);
		std::cout << "asteroid" << std::endl;
		//add to front of linked list
		a->next = g->ahead;
		if (g->ahead != NULL)
			g->ahead->prev = a;
		g->ahead = a;
		g->nasteroids++;
		ab_init();
	}
	clock_gettime(CLOCK_REALTIME, &g->bulletTimer);
	memset(keys, 0, 65536);
}

void normalize(Vec v) {
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0f;
		v[1] = 0.0f;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	int lbutton=0;
	int x,y;
	int rbutton=0;
	if (e->type == ButtonRelease) {
#ifdef COLLISION
		//el_gravity(&e, &g);
		//el_gravity();
		//el_gravity_f(g);
		//g->ship.pos[1] += octogravity;
#endif //COLLISION
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is downi
			lbutton=1;
		}
		if (e->xbutton.button==3) {
			//Right button is down
		rbutton=1;
		if (rbutton){
		}
		}
	}
	x = e->xbutton.x;

	    y = e->xbutton.y;

	        y = yres - y;

	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		//int xdiff = savex - e->xbutton.x;
		//int ydiff = savey - e->xbutton.y;
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
	/*	g->ship.angle += xdiff;
		if (g->ship.angle >= 360.0f)
			g->ship.angle -= 360.0f;
		if (g->ship.angle < 0.0f)
			g->ship.angle += 360.0f;
			*/
		//extern void abcm(XEvent *e);
		cout<<"calling abcm"<<endl;
		abcm(e);
	}


int check_keys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	int key = XLookupKeysym(&e->xkey, 0);
	//
	//This code maintains an array of key status values.
	if (e->type == KeyRelease) {
		keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (e->type == KeyPress) {
		keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
	} else {
		return 0;
	}
	if (shift){}
	switch(key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_h:
			//AB to toggle help menu
			state_help ^= 1;
			ana_show_help();
			break;
		case XK_s:
			//AV to start the game
			state_menu = 0;
			break;
		case XK_m: 
			state_menu=1;
			Analy_show_menu();
			break;
		case XK_r:
			state_menu=1;
			Analy_show_menu();
			break;
		case XK_c:
			//MF to toggle character select screen
			state_charsel ^=1;
			mark_show_charsel();		
			mcheck_keys(e);
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
}

void deleteAsteroid(Game *g, Asteroid *node)
{
	//following example from code/frameworks/asteroids
	if (node->prev == NULL) {
		if(node->next == NULL){//checks the pointer to next asteroid for null so there is not 1 asteroid and action taken
			g->ahead = NULL;
		}
		else {
			node->next->prev = NULL;
			g->ahead = node ->next;
		}

	} else {
		if (node->next == NULL) {
			node->prev->next = NULL;//set to NULL because next is already null
		} else {
			node->prev->next = node->next;//changes link to go around current pointer
			node->next->prev = node->prev;//sets the other link pointing back
		}
	}

	//to do: ^above
	//Delete a node from asteroid linked-list
	delete node;
	node = NULL;//now that pointers are re-pointing around the list item removed delete node.
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0f;
	Flt r2 = ta->radius / 2.0f;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0f) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0f-5.0f;
	ta->pos[1] = a->pos[1] + rnd()*10.0f-5.0f;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0f;
	ta->rotate = a->rotate + (rnd() * 4.0f - 2.0f);
	ta->color[0] = 0.8f;
	ta->color[1] = 0.8f;
	ta->color[2] = 0.7f;
	ta->vel[0] = a->vel[0] + (rnd()*2.0f-1.0f);
	ta->vel[1] = a->vel[1] + (rnd()*2.0f-1.0f);
	//std::cout << "frag" << std::endl;
}

void physics(Game *g)
{
	//EL add sound find a way to sync sounds later
#ifdef SOUND
	//sound does not play all the way through try some sort of time 
	//function to play through whole 9 seconds
	playSound(g->alSourceScuba);
#endif //SOUND
	Flt d0,d1,dist;
	//Update ship position
	g->ship.pos[0] += g->ship.vel[0];
	g->ship.pos[1] += g->ship.vel[1];
#ifdef COLLISION
	//g->ship.vel[1] += el_gravity();
	el_gravity_f(g);
	el_platform_collision(g); 
#endif //COLLISION
	//Check for collision with window edges
	if (g->ship.pos[0] < 0.0f) {
		g->ship.pos[0] += (float)xres;
	}
	else if (g->ship.pos[0] > (float)xres) {
		g->ship.pos[0] -= (float)xres;
	}
	else if (g->ship.pos[1] < 0.0f) {
		g->ship.pos[1] += (float)yres;
	}
	else if (g->ship.pos[1] > (float)yres) {
		g->ship.pos[1] -= (float)yres;
	}
	//
	//
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	for (int i=0; i<g->nbullets; i++) {
		Bullet *b = &g->barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5f) {
			//Delete bullet here.
			memcpy(&g->barr[i], &g->barr[g->nbullets-1], sizeof(Bullet));//changing memory location to quickly delete the bullet and rebuild/position the array
			g->nbullets--;//take away the number of bullets that exist by one now that it is removed from the count
			continue;
		}
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0f) {
			b->pos[0] += (float)xres;
		}
		else if (b->pos[0] > (float)xres) {
			b->pos[0] -= (float)xres;
		}
		else if (b->pos[1] < 0.0f) {
			b->pos[1] += (float)yres;
		}
		else if (b->pos[1] > (float)yres) {
			b->pos[1] -= (float)yres;
		}
		//b = b->next;
	}
	//
	//Update asteroid positions
	Asteroid *a = g->ahead;
	while (a) {
		a->pos[0] += a->vel[0];
		a->pos[1] += a->vel[1];
		//Check for collision with window edges
		if (a->pos[0] < -100.0f) {
			a->pos[0] += (float)xres+200;
		}
		else if (a->pos[0] > (float)xres+100.0f) {
			a->pos[0] -= (float)xres+200.0f;
		}
		else if (a->pos[1] < -100.0f) {
			a->pos[1] += (float)yres+200.0f;
		}
		else if (a->pos[1] > (float)yres+100.0f) {
			a->pos[1] -= (float)yres+200.0f;
		}
		a->angle += a->rotate;
		a = a->next;
	}
	//
	//Asteroid collision with bullets?
	//If collision detected:
	//     1. delete the bullet
	//     2. break the asteroid into pieces
	//        if asteroid small, delete it
	a = g->ahead;
	while (a) {
		//is there a bullet within its radius?
		for (int i=0; i<g->nbullets; i++) {
			Bullet *b = &g->barr[i];
			d0 = b->pos[0] - a->pos[0];
			d1 = b->pos[1] - a->pos[1];
			dist = (d0*d0 + d1*d1);
			if (dist < (a->radius*a->radius)) {
				//std::cout << "asteroid hit." << std::endl;
				//this asteroid is hit.
				if (a->radius > 20.0f) {
					//break it into pieces.
					Asteroid *ta = a;
					buildAsteroidFragment(ta, a);
					int r = rand() % 10 + 5;
					for (int k=0; k<r; k++) {
						//get the next asteroid position in the array
						//Asteroid *ta = new Asteroid;
						//buildAsteroidFragment(ta, a);
						//add to front of asteroid linked list
						//ta->next = g->ahead;
						//if (g->ahead != NULL)
						//g->ahead->prev = ta;
						//g->ahead = ta;
						//g->nasteroids++;
					}
				} else {
					a->color[0] = 1.0f;
					a->color[1] = 0.1f;
					a->color[2] = 0.1f;
					//asteroid is too small to break up
					//delete the asteroid and bullet
					Asteroid *savea = a->next;
					deleteAsteroid(g, a);
					a = savea;
					g->nasteroids--;
				}
				//Delete bullet here.
				//How?
				//Move the array's last element to where this element is.	
				//Then update the array count, nbullets.
				//--like we did with water particles--








				if (a == NULL)
					break;
				continue;
			}
			b = b->next;
		}
		if (a == NULL)
			break;
		a = a->next;
	}
	//---------------------------------------------------
	//check keys pressed now
	if (keys[XK_Left]) {
#ifdef ORIG_PHYSICS
		g->ship.angle += 4.0f;
		if (g->ship.angle >= 360.0f)
			g->ship.angle -= 360.0f;
#endif // ORIG_PHYSICS
#ifdef EL_PHYSICS
		/*g->ship.pos[0] += g->ship.vel[0];
		  g->ship.pos[1] += g->ship.vel[1];*/
		float el_threshold = xres * .25;
		float scroll = 0;
		char direction = 'l';
		g->ship.angle = 0.0f;
		if (g->ship.pos[0] <= el_threshold) {
			g->ship.pos[0] += 0.0f;
			scroll = -4.0f;
		} else {
			g->ship.pos[0] -= 4.0f;
		}
		el_sidescroll(g, scroll,direction);
		//g->ship.angle = 0.0f;
		//g->ship.pos[0] -= 4.0f;
#endif //EL_PHYSICS
	}
	if (keys[XK_Right]) {
#ifdef ORIG_PHYSICS
		g->ship.angle -= 4.0f;
		if (g->ship.angle < 0.0f)
			g->ship.angle += 360.0f;
#endif //ORIG_PHYSICS
#ifdef EL_PHYSICS
		float el_threshold = xres * .75;
		float scroll = 0;
		char direction = 'r';
		g->ship.angle = 0.0f;
		if (g->ship.pos[0] >= el_threshold) {
			g->ship.pos[0] += 0.0f;
			scroll = 4.0f;
		} else {
			g->ship.pos[0] += 4.0f;
		}
		el_sidescroll(g, scroll, direction);
#endif //EL_PHYSICS
	}
	if (keys[XK_Up]) {
		el_jump(g);
#ifdef ORIG_PHYSICS
		//apply thrust
		//convert ship angle to radians
		Flt rad = ((g->ship.angle+90.0f) / 360.0f) * PI * 2.0f;
		//convert angle to a vector
		Flt xdir = cos(rad);
		Flt ydir = sin(rad);
		g->ship.vel[0] += xdir*0.02f;
		g->ship.vel[1] += ydir*0.02f;
		Flt speed = sqrt(g->ship.vel[0]*g->ship.vel[0]+
				g->ship.vel[1]*g->ship.vel[1]);
		if (speed > 10.0f) {
			speed = 10.0f;
			normalize(g->ship.vel);
			g->ship.vel[0] *= speed;
			g->ship.vel[1] *= speed;
		}
#endif //ORIG_PHYSICS
#ifdef EL_PHYSICS
		//change this to some sort of jumping function
#endif //EL_PHYSICS
	}
	if (keys[XK_space]) {
		//a little time between each bullet
		struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&g->bulletTimer, &bt);
		if (ts > 0.1) {
			timeCopy(&g->bulletTimer, &bt);
			//shoot a bullet...
			Bullet *b = &g->barr[g->nbullets];
			timeCopy(&b->time, &bt);
			b->pos[0] = g->ship.pos[0];
			b->pos[1] = g->ship.pos[1];
			b->vel[0] = g->ship.vel[0];
			b->vel[1] = g->ship.vel[1];
			//convert ship angle to radians
			Flt rad = ((g->ship.angle+0.0f) / 360.0f) * PI * 2.0f;//AC: changed firing location to from left to right instead of down to up.
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			b->pos[0] += xdir*20.0f;
			b->pos[1] += ydir*20.0f;
			b->vel[0] += xdir*6.0f + rnd()*0.1f;
			b->vel[1] += ydir*6.0f + rnd()*0.1f;
			b->color[0] = 1.0f;
			b->color[1] = 1.0f;
			b->color[2] = 1.0f;
			g->nbullets++;
		}
	}
}

void render(Game *g)
{
	//el_render_background();
	//float wid;
	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "cs3350 - Armed And Ready");
	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g->nbullets);
	ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g->nasteroids);
	//-------------------------------------------------------------------------
	//AV
	if (state_menu)
		Analy_show_menu();
	//    	physicsB();
	//	renderB();
	//Analy_show_menu();
	//AB
	if (state_help)
		ana_show_help();
	//MF
	if(state_charsel && !state_menu && !state_help)
		mark_show_charsel();

	if (!state_menu && !state_help && !state_charsel) {
		el_render_background();
		//Draw the ship
		glColor3fv(g->ship.color);
		glPushMatrix();
		glTranslatef(g->ship.pos[0], g->ship.pos[1], g->ship.pos[2]);
		//float angle = atan2(ship.dir[1], ship.dir[0]);
		glRotatef(g->ship.angle, 0.0f, 0.0f, 0.0f);// MF set to (0.0f, 0.0f, 0.0f) to straighten
		//	glBegin(GL_TRIANGLES);
		//	glEnable(GL_TEXTURE_2D);
		//	glBegin(GL_QUADS);	
		//glVertex2f(-10.0f, -10.0f);
		//glVertex2f(  0.0f, 20.0f);
		//glVertex2f( 10.0f, -10.0f);

		/*glVertex2f(-12.0f, -10.0f);
		  glVertex2f(  0.0f,  20.0f);
		  glVertex2f(  0.0f,  -6.0f);
		  glVertex2f(  0.0f,  -6.0f);
		  glVertex2f(  0.0f, -20.0f);
		  glVertex2f( 12.0f, -10.0f);*/

		//initCharMark();
		characterSelect();


		//glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);	
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();


		glPopMatrix();
		if (keys[XK_Up]) {
			int i;
			//draw thrust
			Flt rad = ((g->ship.angle+90.0f) / 360.0f) * PI * 2.0f;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			Flt xs,ys,xe,ye,r;
			glBegin(GL_LINES);
			for (i=0; i<16; i++) {
				xs = -xdir * 11.0f + rnd() * 4.0f - 2.0f;
				ys = -ydir * 11.0f + rnd() * 4.0f - 2.0f;
				r = rnd()*40.0f + 40.0f;
				xe = -xdir * r + rnd() * 18.0f - 9.0f;
				ye = -ydir * r + rnd() * 18.0f - 9.0f;
				glColor3f(rnd() * 0.3f + 0.7f, rnd() * 0.3f + 0.7f, 0);
				glVertex2f(g->ship.pos[0]+xs,g->ship.pos[1]+ys);
				glVertex2f(g->ship.pos[0]+xe,g->ship.pos[1]+ye);
			}
			glEnd();
		}
		//-------------------------------------------------------------------------


		//Draw the asteroids
		Asteroid *a = g->ahead;
		while (a) {
			//Log("draw asteroid...\n");
			glColor3fv(a->color);
			glPushMatrix();
			glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
			//glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
			redOctober();
			//initCharMark();
			//glBegin(GL_LINE_LOOP);
			//Log("%i verts\n",a->nverts);
			//for (int j=0; j<a->nverts; j++) {
			//glVertex2f(a->vert[j][0], a->vert[j][1]);
			//}
			//glEnd();
			//glBegin(GL_LINES);
			//	glVertex2f(0,   0);
			//	glVertex2f(a->radius, 0);
			//glEnd();
			glPopMatrix();
			//glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_POINTS);
			glVertex2f(a->pos[0], a->pos[1]);
			glEnd();
			a = a->next;
		}
		//-------------------------------------------------------------------------
		//Draw the bullets
		for (int i=0; i<g->nbullets; i++) {
			Bullet *b = &g->barr[i];
			//Log("draw bullet...\n");
			glColor3f(1.0f, 1.0f, 1.0f);
			projectileTex();
			glBegin(GL_POINTS);//originally 1.0f
			glVertex2f(b->pos[0],      b->pos[1]);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]);
			glVertex2f(b->pos[0],      b->pos[1]-1.0f);
			glVertex2f(b->pos[0],      b->pos[1]+1.0f);
			glColor3f(0.8f, 0.8f, 0.8f);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
			glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
			glEnd();
		}
		//Draw the floor EL
		el_render_floor();
		//init_el_buttons();
	}
}



