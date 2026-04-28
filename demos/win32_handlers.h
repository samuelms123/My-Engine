#ifndef WIN32_HANDLERS_H
#define WIN32_HANDLERS_H

#include <windows.h>

void on_paint(HWND hwnd);
void on_key_w_down();
void on_key_a_down();
void on_key_s_down();
void on_key_d_down();
void on_no_press();
/*
void on_mousemove(LPARAM lparam);
void on_leftmouseup(LPARAM lparam);
void on_rightmousedown(LPARAM lparam);
void on_rightmouseup(LPARAM lparam);
*/

#endif