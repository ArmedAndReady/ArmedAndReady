#ifndef GLOBAL_H
#define GLOBAL_H

#include </usr/include/AL/alut.h>

//extern struct Game game;
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

//=======================================================================
//Setup timers
/*const double physicsRate = 1.0 / 60.0;
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
}*/
//=======================================================================
extern struct timespec timeStart;
extern int xres;
extern int yres;

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
	color[0] = 1.0;
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
    float rotate;
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
//#ifdef SOUND
    ALuint alBufferScuba;
    ALuint alSourceScuba;
    ALuint alBufferShoot;
    ALuint alSourceShoot;
//#endif //SOUND
    Game() {
	ahead = NULL;
	barr = new Bullet[MAX_BULLETS];
	nasteroids = 0;
	nbullets = 0;
    }
};




#endif
