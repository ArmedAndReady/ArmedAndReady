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
GLuint rSil; // */rOTex;
GLuint AtkTex;
#ifdef SOUND
extern ALuint alBufferShoot;
extern ALuint alSourceShoot;
#endif

extern int xres;
extern int yres;
extern int state_charsel;
static int charsel = 0;
int mdone = 0;
extern int keys[];
void mark_show_charsel();
int mcheck_keys(XEvent *e);
void mmmm(XEvent *e);
void initCharMark();    //draw default charcter
void redOctober();      //use this to draw enemy
void projectileTex();   //use this to draw projectiles
void texGen();          //generates all textures
void characterSelect(); //select character sprite
unsigned char *buildAlphaData(Ppmimage *img, unsigned char trans_col[3]);
void markButtons();

typedef struct c_button {
        Rect r;
        char text[32];
        int over;
        int down;
        int click;
        float color[3];
        float dcolor[3];
        unsigned int text_color;

} Button;

Button mbutton[3];

#ifdef SOUND
extern void initSound(Game *g);
extern void cleanupSound(Game *g);
extern void playSound(ALuint source);
extern int sound;
void initattacksound(Game *g);
void attacksound(ALuint source);
int soundcheck = 0;
#endif

#ifdef SOUND
void initattacksound(Game *g)
{
        g->alBufferShoot = alutCreateBufferFromFile("attack.wav");
        alGenSources(1, &g->alSourceShoot);
        alSourcei(g->alSourceShoot, AL_BUFFER, g->alBufferShoot);
        alSourcef(g->alSourceShoot, AL_GAIN, 1.0f);
        alSourcef(g->alSourceShoot, AL_PITCH, 1.0f);
        alSourcef(g->alSourceShoot, AL_LOOPING, AL_TRUE);
}

void attacksound(ALuint source)
{
        cout << soundcheck << endl;
        //cout << "In attack sound...\n";
        while (soundcheck == 1) {
                cout << "In attacksound loop...\n";
                //play sound
                alSourcePlay(source);
                soundcheck = 0;
        }
}
#endif
int moffset = 105;
int moffsetx = 150;
int moffsety = 200;
void mark_show_charsel()
{
        //cout << "Showing character select. . .\n";
        Rect r;
        int x = xres;
        int y = yres;
        if (x||y){};
        glColor3f(0.0,1.0,1.0);
        glBegin(GL_QUADS);
        glVertex2i((xres-100),(100));
        glVertex2i((xres-100),(yres-100));
        glVertex2i((100), (yres-100));
        glVertex2i((100), (100));
        glEnd();
        glPopMatrix();
        r.bot = 750;
        r.left = 555;
        r.center = 0;
        ggprint16(&r, 96, 0x0000ff, "CHARACTER SELECT");

        glColor3i(255,255,0);
        for (int i=0; i<3; i++) {
                if(mbutton[i].over){
                        glLineWidth(2);
                        glBegin(GL_LINE_LOOP);
                        glVertex2i(mbutton[i].r.left-2,  mbutton[i].r.bot-2);
                        glVertex2i(mbutton[i].r.left-2,  mbutton[i].r.top+2);
                        glVertex2i(mbutton[i].r.right+2, mbutton[i].r.top+2);
                        glVertex2i(mbutton[i].r.right+2,  mbutton[i].r.bot-2);
                        glVertex2i(mbutton[i].r.left-2,  mbutton[i].r.bot-2);
                        glEnd();
                        glLineWidth(1);
                        glColor3fv(mbutton[i].dcolor);
                }
                else{
                        glColor3f(1.0, 1.0, 1.0);
                }
                glBegin(GL_QUADS);
                glVertex2i((xres-moffsetx), ((moffsety)*3)-i*moffset);
                glVertex2i((xres-moffsetx), (yres-moffsety)-i*moffset);
                glVertex2i((moffsetx), (yres-moffsety)-i*moffset);
                glVertex2i((moffsetx), ((moffsety)*3)-i*moffset);
                glEnd();
        }
        r.bot = 645;
        r.left = 555;
        r.center = 0;
        ggprint16(&r, 0, 0x0000ff, "1 - Red October");
        r.bot = 540;
        r.left = 555;
        r.center = 0;
        ggprint16(&r, 0, 0x0000ff, "2 - Dirt Texture");
        r.bot = 435;
        r.left = 555;
        r.center = 0;
        ggprint16(&r, 0, 0x0000ff, "0 - Default Texture");
}
void mmmm(XEvent *e)
{
        static int savex = 0;
        static int savey = 0;
        int x, y;
        int lbutton=0;
        if (lbutton){};
        int rbutton=0;
        if(e->type == ButtonRelease){
                return;
        }
        if (e->type == ButtonPress){
                if (e->xbutton.button == 1) {
                        lbutton = 1;
                }
                if (e->xbutton.button == 3) {
                        rbutton = 1;
                        if (rbutton) {}
                }
        }

        x = e->xbutton.x;
        y = e->xbutton.y;
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
                savex = e->xbutton.x;
                savey = e->xbutton.y;
        }

        int nbuttons = 3;
        for (int i=0; i<nbuttons; i++) {
                mbutton[i].over = 0;
                if(x >= mbutton[i].r.centerx - mbutton[i].r.width &&
                                x <= mbutton[i].r.centerx + mbutton[i].r.width &&
                                y >= mbutton[i].r.centery - mbutton[i].r.height &&
                                y <= mbutton[i].r.centery + mbutton[i].r.height) {
                        mbutton[i].over=1;
                        if(mbutton[i].over) {
                                if (lbutton) {
                                        switch(i) {
                                                case 0:
                                                        charsel = 1;
                                                        break;
                                                case 1:
                                                        charsel = 2;
                                                        break;
                                                case 2:
                                                        charsel = 0;
                                                        break;

                                        }
                                }
                        }

                }
        }
}

void markButtons(){
        //int offsets[3];
        int ttop[3];
        int tbottom[3];
        for(int i=0; i<3; i++){
                tbottom[i] = moffsety*3 - i*moffset;
                ttop[i]=yres-moffsety-i*moffset;
                //offsets[i]=(top[i]+bottom[i])/2;      
        }
        int nbuttons = 0;
        mbutton[nbuttons].r.width=475;
        mbutton[nbuttons].r.height=50;
        mbutton[nbuttons].r.centerx = (float)xres/2.0;
        mbutton[nbuttons].r.centery = 200;

        mbutton[nbuttons].r.left=150;
        mbutton[nbuttons].r.right=xres-150;
        mbutton[nbuttons].r.top=ttop[nbuttons];
        mbutton[nbuttons].r.bot=tbottom[nbuttons];
        mbutton[nbuttons].click=0;
        mbutton[nbuttons].over=0;
        mbutton[nbuttons].down=0;
        mbutton[nbuttons].dcolor[0]=0.0f+0.5;
        mbutton[nbuttons].dcolor[1]=0.4f;
        mbutton[nbuttons].dcolor[2]=0.7f;

        nbuttons++;
        mbutton[nbuttons].r.width=475;
        mbutton[nbuttons].r.height=50;
        mbutton[nbuttons].r.centerx = (float)xres/2.0;
        mbutton[nbuttons].r.centery = 355;

        mbutton[nbuttons].r.left=150;
        mbutton[nbuttons].r.right=xres-150;
        mbutton[nbuttons].r.top=ttop[nbuttons];
        mbutton[nbuttons].r.bot=tbottom[nbuttons];
        mbutton[nbuttons].click=0;
        mbutton[nbuttons].over=0;
        mbutton[nbuttons].down=0;
        mbutton[nbuttons].dcolor[0]=0.0f+0.5;
        mbutton[nbuttons].dcolor[1]=0.4f;
        mbutton[nbuttons].dcolor[2]=0.7f;

        nbuttons++;
        mbutton[nbuttons].r.width=475;
        mbutton[nbuttons].r.height=50;
        mbutton[nbuttons].r.centerx = (float)xres/2.0;
        mbutton[nbuttons].r.centery = 460;

        mbutton[nbuttons].r.left=150;
        mbutton[nbuttons].r.right=xres-150;
        mbutton[nbuttons].r.top=ttop[nbuttons];
        mbutton[nbuttons].r.bot=tbottom[nbuttons];
        mbutton[nbuttons].click=0;
        mbutton[nbuttons].over=0;
        mbutton[nbuttons].down=0;
        mbutton[nbuttons].dcolor[0]=0.0f+0.5;
        mbutton[nbuttons].dcolor[1]=0.4f;
        mbutton[nbuttons].dcolor[2]=0.7f;
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
        //Currently works for transparency, but too taxing when applied to more objects

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
        //          shift+V (enters visual line)
        //          scroll down to bottom to highlight everything
        //          "<" to remove indents
        //          "." to repeat in case
        //          "gg=G"
        //  unlink("./Game_Tiles/octopus.ppm");
}

void redOctober()
{
        //    string debug = "Printing character to screen...\n";
        //    cout << debug;

        int w = redImage->width;
        int h = redImage->height;
        float wid = 64.0f;

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        /*    glBindTexture(GL_TEXTURE_2D, rOTex);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
              glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
              glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, redImage->data);
         */
        glBindTexture(GL_TEXTURE_2D, rSil);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char tc[3] = {180, 250, 188};
        unsigned char *rSil = buildAlphaData(redImage, tc);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rSil);
        free(rSil);

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
        glGenTextures(1, &rSil);// */&rOTex);
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
