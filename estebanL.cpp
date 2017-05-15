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
int score = 0;
extern int xres;
extern int yres;
extern Ppmimage *floorImage;
extern GLuint floorTexture; 
extern Ppmimage * parallaxImage;
extern GLuint parallaxTexture;
//extern const float octogravity;
//extern struct game->ship.vec;
//extern struct Game game;
//extern Game game;
//extern int game->nasteroids;
const float octogravity = -0.006f;
bool collided = false;
int floor_set = 0;
int checked = 0;
int reset_boxes = 0;
float shift_platform = 0;
int el_enemy_count = 0;
//int score = 0;

extern "C" {
#include "fonts.h"
}
//void read_by_char(string);

//struct Vec{
//      float x, y, z;
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
Shape stat_box[3];
Shape life_box[10];
int set_from_text[MAX_BLOCKS_WIDE];


void read_from_text();
void set_platform_boxes();
void draw_boxes();
void check_position();
void reset_platforms();
void set_background();
void draw_background();
void el_stats();
void set_stats_box();
void draw_stats_box();
void set_life_boxes();
//void el_sidescroll();
//Have Mark use if images are set and set to 1
bool image_set = 0;
float octo_position = 0.0;
int high_scores_read = 0;

using namespace std;

typedef struct t_el_button {
    Rect r;
    char text[32];
    int over;
    int down;
    int click;
    float color[3];
    float dcolor[3];
    unsigned int text_color;
} El_Button;

El_Button el_button[2];

void init_el_buttons()
{
    int nbuttons=0;
    //size and position
    el_button[nbuttons].r.width = 140;
    el_button[nbuttons].r.height = 60;
    el_button[nbuttons].r.left = 20;
    el_button[nbuttons].r.bot = 320;
    el_button[nbuttons].r.right =
        el_button[nbuttons].r.left + el_button[nbuttons].r.width;
    el_button[nbuttons].r.top =
        el_button[nbuttons].r.bot + el_button[nbuttons].r.height;
    el_button[nbuttons].r.centerx =
        (el_button[nbuttons].r.left + el_button[nbuttons].r.right) / 2;
    el_button[nbuttons].r.centery =
        (el_button[nbuttons].r.bot + el_button[nbuttons].r.top) / 2;
    strcpy(el_button[nbuttons].text, "Reset");
    el_button[nbuttons].down = 0;
    el_button[nbuttons].click = 0;
    el_button[nbuttons].color[0] = 0.4f;
    el_button[nbuttons].color[1] = 0.4f;
    el_button[nbuttons].color[2] = 0.7f;
    el_button[nbuttons].dcolor[0] = el_button[nbuttons].color[0] * 0.5f;
    el_button[nbuttons].dcolor[1] = el_button[nbuttons].color[1] * 0.5f;
    el_button[nbuttons].dcolor[2] = el_button[nbuttons].color[2] * 0.5f;
    el_button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;
    el_button[nbuttons].r.width = 140;
    el_button[nbuttons].r.height = 60;
    el_button[nbuttons].r.left = 20;
    el_button[nbuttons].r.bot = 160;
    el_button[nbuttons].r.right =
        el_button[nbuttons].r.left + el_button[nbuttons].r.width;
    el_button[nbuttons].r.top = el_button[nbuttons].r.bot +
        el_button[nbuttons].r.height;
    el_button[nbuttons].r.centerx = (el_button[nbuttons].r.left +
            el_button[nbuttons].r.right) / 2;
    el_button[nbuttons].r.centery = (el_button[nbuttons].r.bot +
            el_button[nbuttons].r.top) / 2;
    strcpy(el_button[nbuttons].text, "Quit");
    el_button[nbuttons].down = 0;
    el_button[nbuttons].click = 0;
    el_button[nbuttons].color[0] = 0.3f;
    el_button[nbuttons].color[1] = 0.3f;
    el_button[nbuttons].color[2] = 0.6f;
    el_button[nbuttons].dcolor[0] = el_button[nbuttons].color[0] * 0.5f;
    el_button[nbuttons].dcolor[1] = el_button[nbuttons].color[1] * 0.5f;
    el_button[nbuttons].dcolor[2] = el_button[nbuttons].color[2] * 0.5f;
    el_button[nbuttons].text_color = 0x00ffffff;
    nbuttons++;

}

void print_esteban()
{       
    string esteban = "Esteban";
    cout << esteban << endl;
}


#ifdef FLOOR_STUDY
void el_render_floor()
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

void el_render_background()
{

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
            //      cout << "DEBUG: boxes\n";
            //      g->ship.pos[0] = platform_boxes[i].x +
            //                platform_boxes[i].width;
            g->ship.pos[1] = platform_boxes[i].y +
                platform_boxes[i].height * 1.01;
            collided =  true;       
            //g->ship.vel[1] = 0;
        }
    }       
}
#endif //FLOOR_STUDY

#ifndef FLOOR_STUDY
int set = 0;

void el_render_floor(){
    if (!set) {
        read_from_text();
        set_platform_boxes();
        //set = 1;
    }
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

void el_render_background()
{
    set_background();
    draw_background();
}

void set_background()
{
    //extern Ppmimage * parallaxImage;
    //extern GLuint parallaxTexture;


}

void draw_background()
{
    float w, h;
    //      w = xres;
    //      h = yres;
    glColor3ub(0, 125, 204);
    glBindTexture(GL_TEXTURE_2D, parallaxTexture);
    glPushMatrix();
    glBegin(GL_QUADS);
    /*      glTexCoord2f(0.0, 1.0);
            glVertex2i(0,0);
            glTexCoord2f(0.0, 0.0); 
            glVertex2i(0,h);
            glTexCoord2f(1.0, 0.0);
            glVertex2i(w,h);
            glTexCoord2f(1.0, 1.0);
            glVertex2i(w,0);
            */

    w = (1920.0/2048.0);
    h = (1080.0/2048.0);
    glTexCoord2f(0.0, h);
    glVertex2i(0,0);
    glTexCoord2f(0.0, 0.0); 
    glVertex2i(0,yres);
    glTexCoord2f(w, 0.0);
    glVertex2i(xres,yres);
    glTexCoord2f(w, h);
    glVertex2i(xres,0);
    glEnd();
    glPopMatrix();
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
        checked = 1;
    }
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
        //platform_boxes[box_num].center[0] = pix_center + (i*PIXEL_WIDTH);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        float position;
        for (int j = 0 ; j < set_from_text[i]; j++) {
            //Need this variable for K&R line length
            position = pix_center + (j*PIXEL_WIDTH);
            if (position){};
            glPushMatrix();
            glBegin(GL_QUADS);
            //platform_boxes[box_num].center[1] = position; 
            //#define ORIG_SETUP
#ifdef ORIG_SETUP
            glTexCoord2f(0.0, 1.0);
            glVertex2i(w*i,h*j);
            glTexCoord2f(0.0, 0.0); 
            glVertex2i(w*i,h*j+pixels);
            glTexCoord2f(1.0, 0.0);
            glVertex2i(w*i+pixels,h*j+pixels);
            glTexCoord2f(1.0, 1.0);
            glVertex2i(w*i+pixels,h*j);
#endif //ORIG_SETUP
#define SETUP_TEST
#ifdef SETUP_TEST
            //float shift_platform = g->ship.pos[0];
            //w = platform_boxes[box_num].center[0];
            //h = platform_boxes[box_num].center[1]; 
            glTexCoord2f(0.0, 1.0);
            glVertex2i((w*i)-shift_platform,h*j);
            glTexCoord2f(0.0, 0.0); 
            glVertex2i((w*i)-shift_platform,h*j+pixels);
            glTexCoord2f(1.0, 0.0);
            glVertex2i((w*i+pixels)-shift_platform,h*j+pixels);
            glTexCoord2f(1.0, 1.0);
            glVertex2i((w*i+pixels)-shift_platform,h*j);
#endif //SETUP_TEST
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
    int check_x_collision = false;
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
                //      cout << "DEBUG: boxes\n";
                //      g->ship.pos[0] = platform_boxes[i].x +
                //                platform_boxes[i].width;
                //#define DETECT_COLLISION
#ifdef DETECT_COLLISION 
                cout <<"DEBUG: in el_platform_collision\n";
                cout <<"Collision detected with platform " << i + 1 << endl;
#endif //DETECT_COLLISION

                g->ship.pos[1] = platform_boxes[i].center[1] +
                    platform_boxes[i].height * 0.83;
                collided =  true;       
                //g->ship.vel[1] = 0;
            }
        }
        if (check_x_collision) {
        }
    }       
}
#endif //#ifndef FLOOR_STUDY

void el_check_x_collision(Game *g, char dir)
{
    switch(dir){
        case 'l':
            for (int i = total_boxes; i > 0; i--) {
                if(g->ship.pos[0] <= platform_boxes[i].center[0] +
                        platform_boxes[i].width  && 
                        g->ship.pos[0] >= platform_boxes[i].center[0] -
                        platform_boxes[i].width &&
                        g->ship.pos[1] >= platform_boxes[i].center[1] +
                        platform_boxes[1].height * 0.83 /*&& 
                                                          g->ship.pos[1] >= platform_boxes[i].center[1] - 
                                                          platform_boxes[1].height * 0.83*/) {
                    g->ship.pos[0] = platform_boxes[i].center[0] +
                        platform_boxes[i].width /** 0.83*/;
                }
            }
            break;

        case 'r':
            for (int i = total_boxes; i > 0; i--) {
                if(g->ship.pos[0] <= platform_boxes[i].center[0] +
                        platform_boxes[i].width && 
                        g->ship.pos[0] >= platform_boxes[i].center[0] -
                        platform_boxes[i].width &&
                        g->ship.pos[1] >= platform_boxes[i].center[1] +
                        platform_boxes[1].height * 0.83/* && 
                                                          g->ship.pos[1] >= platform_boxes[i].center[1] - 
                                                          platform_boxes[1].height * 0.83*/) {
                    g->ship.pos[0] = platform_boxes[i].center[0] -
                        platform_boxes[i].width /** 0.83*/;
                }               
            }
            break;
    }
}

void check_position()
{
    /*      if(g->ship.pos[0] > xres * 0.75){       
            cout <<"TEST: Octo past 75 percent on screen\n";
            }*/
}

void reset_platforms(float shift)
{
    float reset = shift;
    //#define OKIEDOKIE
#ifdef OKIEDOKIE
    char okiedokie = ' ';
    cout << "Reset/Shift = " << reset << endl;
    cout <<"TEST: checked shift";
    cin >> okiedokie;
#endif //OKIEDOKIE
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
    if (reset_boxes) {
        for (int i = 0 ; i < total_boxes; i++) {
            //platform_boxes[num].center[0]= w+(PIXEL_WIDTH*i);
            //float a = platform_boxes[num].center[0];      
            for (int j = 0; j < set_from_text[i]; j++) {
                platform_boxes[num].center[0]= (w+(PIXEL_WIDTH*i))-reset;
                platform_boxes[num].center[1]=h+(PIXEL_WIDTH*j);
                float a = platform_boxes[num].center[0];        
                float b = platform_boxes[num].center[1];
                if (a||b){};
                //#define CHECK_PB_BOXES_RESET
#ifdef CHECK_PB_BOXES_RESET
                int checked_reset = 0;
                //#define MANUAL_TEST
#ifdef MANUAL_TEST
                char keep_going = ' ';
                cout << "TEST: CONTINUE? ";
                cin >> keep_going;
#endif //MANUAL_TEST
                if(!checked_reset) {
                    cout<<"DEBUG: Platform num = : "<<num << endl;
                    cout<<"DEBUG: Platform x = : "<<a << endl;
                    cout<<"DEBUG: Platform y = : "<<b << endl;
                }
#endif //CHECK_PB_BOXES_RESET
                num++;
            }
        }
        reset_boxes = 0;
    }
    draw_boxes();
}

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
    /*shift_platform = g->ship.pos[0];
      if(g->ship.pos[0] > xres * 0.75) {
      reset_boxes = 1;
      reset_platforms(shift_platform);
      reset_boxes = 0;*/
    //g->ship.pos[0] -= shift_platform;
    //reset_boxes = 1;
    //cout <<"TEST: Octo past 75 percent on screen\n";
    //reset_platforms(); move both octopus and platforms back and introduce new ones.
    //}
}

void el_sidescroll(Game *g, float scroll, char dir)
{
    //shift_platform = g->ship.pos[0];
    float threshold = 0.0f;
    switch(dir) {
        case 'l':
            threshold =  xres * 0.25;
            if(g->ship.pos[0] < threshold) {
                reset_boxes = 1;
                shift_platform += scroll;
                reset_platforms(shift_platform);
                reset_boxes = 0;
            }
        case 'r':
            threshold =  xres * 0.75;
            if(g->ship.pos[0] > threshold) {
                reset_boxes = 1;
                shift_platform += scroll;
                reset_platforms(shift_platform);
                reset_boxes = 0;
            }
    }
}

void el_jump(Game *g)
{   
    //seems more like a swim up rather than a jump
    //might be ideal for this game  
    //g->ship.vel[1] -= octogravity * 3;
    collided = false;
    if(g->ship.pos[1] >= yres) {
        g->ship.vel[1] = 0.0f;
    } else {
        g->ship.vel[1] -= octogravity * 3;
    }
}
//void read_by_char(string filename)

bool stats_box_set = 0;
bool life_box_set = 0;

void el_stats()
{
    if (!stats_box_set) {
        set_stats_box();
        stats_box_set = 1;
    }
    if (!life_box_set) {
        set_life_boxes();
        life_box_set = 1;
    }
    draw_stats_box();
}

void set_stats_box() 
{
    stat_box[0].width = 100;
    stat_box[0].height = 50;
    stat_box[0].center[0] = 1135;
    stat_box[0].center[1] = 100;
}

void set_life_boxes()
{
    Shape *s;
    s = &stat_box[0];
    for (int i = 0; i < 10; i++) {
        life_box[i].width = 4;
        life_box[i].height = 15;
        life_box[i].center[0] = s->center[0] + (i*9);
        life_box[i].center[1] = s->center[1] + 10;
    }
}

int dummy_life = 10;

void draw_life_boxes()
{       
    Shape *s;
    float w, h;
    int count = 0;
    switch (dummy_life) {
        case 1:
        case 2:
        case 3:
            glColor3ub(255,0,0);
            break;
        case 4:
        case 5:
        case 6: 
            glColor3ub(255,153,0);
            break;
        case 7:
        case 8:
        case 9:
            glColor3ub(255,255,0);
            break;
        default:
            glColor3ub(0,255,0);
            break;

    }
    //glColor3ub(0,255,0);

    while(count < dummy_life) {
        s = &life_box[count];
        glPushMatrix();
        glTranslatef(s->center[0], s->center[1], s->center[2]);
        w = s->width;
        h = s->height;
        glBegin(GL_QUADS);
        glVertex2i(-w,-h);
        glVertex2i(-w,h);
        glVertex2i(w,h);
        glVertex2i(w,-h);
        glEnd();
        glPopMatrix();
        count++;
    }
}

int dummy_score = 1000;

void draw_stats_box()
{
    float w, h;
    Rect r;
    Shape *s;
    s = &stat_box[0]; 
    //glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(s->center[0], s->center[1], s->center[2]);
    w =  s->width;
    h =  s->height;
    /*glTranslatef(s.center[0], s->center[1], s->center[2]);
      w =  s->width;
      h =  s->height;*/
    glBegin(GL_QUADS);
    glVertex2i(-w,-h);
    glVertex2i(-w,h);
    glVertex2i(w,h);
    glVertex2i(w,-h);
    glEnd();
    glPopMatrix();

    r.bot = s->center[1] + 25;
    r.left = s->center[0] - 90;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Score");

    r.bot = s->center[1] + 25;
    r.left = s->center[0];
    r.center = 0;
    //ggprint16(&r, 0, 0x00ffffff, "%i", dummy_score);
    ggprint16(&r, 0, 0x00ffffff, "%i", score);

    draw_life_boxes();

    r.bot = s->center[1];
    r.left = s->center[0] - 90;
    r.center = 0;
    ggprint16(&r, 0, 0x00ffffff, "Life");
}

void el_update_score(char c)
{
    switch(c) {
        case 'e':
            score += 100;
            break;
    }

}

int injury_effect = 0;

void el_enemy_collision(Game *g)
{
    float d0, d1, dist;
    Asteroid *w;
    w = g->ahead;
    if (g) {};
    int checked = 0;
    if (checked) {};
#define ENEMY_COLLISION1
#ifdef ENEMY_COLLISION1
    while (w) {
        //cout << "DEBUG: in enemy collision loop \n";
        for (int i = 0; i < el_enemy_count; i++) {
            d0 = g->ship.pos[0] - w->pos[0];
            d1 = g->ship.pos[1] - w->pos[1];
            dist = (d0*d0 + d1*d1);
            if (dist < (w->radius*w->radius)) {
                w->vel[0] = g->ship.vel[0];
                //w->vel[1] = (-1.0) * (w->vel[1]);

                if (injury_effect == 0)
                {
                    dummy_life--;
                    injury_effect = 1000;
                } else { 
                    injury_effect--;
                }
            }
        }
        w = w->next;
    }
#endif //ENEMY_COLLISION1
    //#define ENEMY_COLLISION
#ifdef ENEMY_COLLISION
    cout << "DEBUG: enemy collision\n";
    cout << "DEBUG: enemy count = " << el_enemy_count  << endl;
#endif //ENEMY_COLLISION
}

int high_scores[10];

void el_read_high_scores(){
    string filename = "topten.txt";
    ifstream ifs;
    int number;
    ifs.open(filename.c_str());
    total_boxes = 0;
    //while (ifs >> number && !ifs.eof()) {
    for (int i = 0; i < 10; i++) {
        ifs >> number;
        high_scores[i] = number;
    }
    ifs.close();
    bool check = 0;
    if (check) {
        for (int i = 0; i < 10; i++) {
            cout << "High Score " << i << ": " << high_scores[i] << endl;
        }
    }
}

int updated_high_scores[10];

void el_update_high_scores()
{
    bool higher_score = 0;
    for (int i = 0; i < 10;  i++ ) {
        if (score > high_scores[i]) {
            updated_high_scores[i] = score;
            higher_score = 1;
            i++;
        } else if (higher_score) {
            updated_high_scores[i] = high_scores[i-1];
        } else {
            updated_high_scores[i] = high_scores[i];
        }
    }
    if (higher_score) {

    }       
}


