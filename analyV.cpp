//Analy Velazquez
//Armed and Ready

#include <iostream>
#include <cstring>
#include <stdlib.h>
//#include <GL/glx.h>
extern "C" {
#include "fonts.h"
}


using namespace std;



void print_Analy() {
    string analy= "analy";
    cout << analy << endl;
}

void Analy_show_menu()
{
    Rect r;
    r.bot = 400;
    r.left = 400;
    r.center = 0;

    ggprint8b(&r, 16, 0x00ff0000, "Start menu");
    ggprint8b(&r, 16, 0x00ff0000, "Press S to start");
    
}


