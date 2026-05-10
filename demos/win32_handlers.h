#ifndef WIN32_HANDLERS_H
#define WIN32_HANDLERS_H

#include <windows.h>
#include "../include/myngn_physics/myngn_math.h"

void on_paint(HWND hwnd);
void on_key_w_down();
void on_key_a_down();
void on_key_s_down();
void on_key_d_down();
void on_key_space_down();
void on_no_press();
void on_mouse_click(myVec2 mouse_pos);
/*
void on_mousemove(LPARAM lparam);
void on_leftmouseup(LPARAM lparam);
void on_rightmousedown(LPARAM lparam);
void on_rightmouseup(LPARAM lparam);
*/

#endif