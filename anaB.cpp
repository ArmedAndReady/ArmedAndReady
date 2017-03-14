// Author:Ana Butanda 
// Date: Spring 2017
// Purpose: Help screen for Armed and Ready 
//
#include <iostream>
#include <cstring>
#include <stdlib.h>
extern"C"{
#include "fonts.h"
}
using namespace std;
void print_Ana()
{
    string ana ="ana" ;
    cout << ana << endl;

}

void ana_show_help()
{
    Rect r;
    r.bot = 400;
    r.left = 400;
    r.center = 0;
    ggprint8b(&r,16,0x00ff0000, "Help Screen- Prototype");
    //
    ggprint8b(&r, 16,0x00ff000,"Press Hto toggle");

}

