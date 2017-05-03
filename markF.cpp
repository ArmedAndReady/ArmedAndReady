// Author: Mark Felisilda
// Date: Spring 2017
// Purpose: Image Conversion/Animation for Armed and Ready

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "ppm.h"
#include "log.h"
#include "global.h"
extern "C" {
#include "fonts.h"
}

#define SOUND
#ifdef SOUND
#include </usr/include/AL/alut.h>
#endif

using namespace std;

Ppmimage *octoImage=NULL;
Ppmimage *redImage=NULL;
Ppmimage *AtkImage=NULL;
GLuint octoSil;// */octoTexture;
GLuint rOTex;
GLuint AtkTex;

extern int xres;
extern int yres;
extern int state_charsel;
static int charsel = 0;
int mdone = 0;
extern int keys[];
void mark_show_charsel();
int mcheck_keys(XEvent *e);
void initCharMark();	//draw default charcter
void redOctober();	//use this to draw enemy
void projectileTex();	//use this to draw projectiles
void texGen();		//generates all textures
void characterSelect(); //select character sprite
unsigned char *buildAlphaData(Ppmimage *img, unsigned char trans_col[3]);

#ifdef SOUND
extern void initSound(Game *g);
extern void cleanupSound(Game *g);
extern void playSound(ALuint source);
extern int sound;
#endif

void mark_show_charsel()
{
    //cout << "Showing character select. . .\n";
    Rect r;
    glColor3ub(0,255,255);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(0,yres);
    glVertex2i(xres, yres);
    glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
    r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint8b(&r, 96, 0x0000ff, "CHARACTER SELECT");
    ggprint8b(&r, 48, 0x0000ff, "1 - Red October");
    ggprint8b(&r, 48, 0x0000ff, "2 - Dirt Texture");
    ggprint8b(&r, 48, 0x0000ff, "0 - Default Texture");
}

int mcheck_keys(XEvent *e)
{
    //cout << "Set done to 1\n";
    mdone = 1;
    //cout << "Checking keys. . .\n";
    int mkey = XLookupKeysym(&e->xkey, 0);
    //cout << "int key initialized...\n";

    //cout << "Right before switch...\n";
    if(state_charsel != 0){
	switch(mkey) {
	    case XK_0:
		charsel = 0;
		break;
	    case XK_1:
		//cout << "Set charsel to 1\n";
		charsel = 1;
		break;
	    case XK_2:
		charsel = 2;
		break;
	}
    }
    characterSelect();
    return 0;

}

void characterSelect()
{
    //        cout << charsel;
    if(charsel == 0)
	initCharMark();
    else if(charsel == 1)
	redOctober();
    else if(charsel == 2)
	projectileTex();
}

void initCharMark()
{
    //    string debug = "Printing character to screen...\n";
    //    cout << debug;

    int w = octoImage->width;
    int h = octoImage->height;
    float wid = 64.0f;

    //    glColor3f(0.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    /*    glBindTexture(GL_TEXTURE_2D, octoTexture);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, octoImage->data);
	  */
    glBindTexture(GL_TEXTURE_2D, octoSil);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char tc[3] = {180, 250, 188};
    unsigned char *octoSil = buildAlphaData(octoImage, tc);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, octoSil);
    free(octoSil);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();

    // 04/18 (11:52): does image conversion, draws character tile, 
    // but does not put texture on tile  
    // 04/18 (13:06): tile textured 
    // comment: I've worked on this but github hasn't been
    // recording my pushes to origin master. You can check the logs. 
    // Mark Felisilda has pushed/committed multiple times
    // 4/24 (08:26): tabs should be fixed
    // in case:
    // 		shift+V (enters visual line)
    // 		scroll down to bottom to highlight everything
    // 		"<" to remove indents
    // 		"." to repeat in case
    // 		"gg=G"
    //	unlink("./Game_Tiles/octopus.ppm");
}

void redOctober()
{
    //    string debug = "Printing character to screen...\n";
    //    cout << debug;

    int w = redImage->width;
    int h = redImage->height;
    float wid = 64.0f;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, rOTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, redImage->data);

    glBegin(GL_QUADS); 
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();
}

void projectileTex()
{
    int w = AtkImage->width;
    int h = AtkImage->width;
    float wid = 32.0f;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, AtkTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, AtkImage->data);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,  wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid,  wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid, -wid);
    glEnd();

}

void texGen()
{
    //Player character ppm creation
    //system("convert ./Game_Tiles/octopus.png ./Game_Tiles/octopus.ppm");
    system("convert ./Game_Tiles/Octopus.png ./Game_Tiles/octopus.ppm");
    //system("convert ./Game_Tiles/Tracer.png ./Game_Tiles/octopus.ppm");
    octoImage = ppm6GetImage("./Game_Tiles/octopus.ppm");
    glGenTextures(1, &octoSil);// */&octoTexture);
    unlink("./Game_Tiles/octopus.ppm");

    //Enemy character ppm creation
    system("convert ./Game_Tiles/RedOctober.png ./Game_Tiles/redOctober.ppm");
    redImage = ppm6GetImage("./Game_Tiles/redOctober.ppm");
    glGenTextures(1, &rOTex);
    unlink("./Game_Tiles/redOctober.ppm");

    //Attack projectile texture creation
    system("convert ./Game_Tiles/Dirt_1.png ./Game_Tiles/Atk.ppm");
    AtkImage = ppm6GetImage("./Game_Tiles/Atk.ppm");
    glGenTextures(1, &AtkTex);
    unlink("./Game_Tiles/Atk.ppm");
}

unsigned char *buildAlphaData(Ppmimage *img, unsigned char trans_col[3])
{
    //Transparency color RGB value: (180, 250, 188)
    int i;
    int a, b, c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    newdata = (unsigned char *)malloc(img->width * img->height * 4);
    ptr = newdata;
    for (i=0; i < img->width * img->height * 3; i+=3) {
	a = *(data+0);
	b = *(data+1);
	c = *(data+2);
	*(ptr+0) = a;
	*(ptr+1) = b;
	*(ptr+2) = c;
	*(ptr+3) = 1;
	if (a==trans_col[0] &&
		b==trans_col[1] &&
		c==trans_col[2]) {
	    *(ptr+3) = 0;
	}
	ptr += 4;
	data += 3;
    }

    return newdata;
} 

void print_Mark()
{
    string Mark = "Mark\n";
    cout << Mark;
}
