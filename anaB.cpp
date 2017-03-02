// Author:Ana Butanda 
// Date: Spring 2017
// Purpose: Help screen for Armed and Ready 
//
#include <iostream>
#include <cstring>
#include <stdlib.h>

using namespace std;
void print_Ana()
{
    string ana ="ana" ;
    cout << ana << endl;

}

/*void checkKeys(XEvent *e)
{
    //keyboard input 
    static int shift = 0;
    int key=XLookupKeyssym(&e->xkey,0);
    if (e->type == KeyRealease){
	if (key == XK_Shift_L || key == XK_shift_R)
	    shift =0;
	return 0;
    }
    if (e->type == KeyPress){
	if (key ==XK_Shift_l || key == XK_Shift_R){
	    shift=1;
	    return ;
	}
    }else{
	return;
    }
    switch(key){
	case XK_`:
	    state_help ^=1;
	    break;
    }
}
void render(void)
{

*/
