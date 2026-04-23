#ifndef WIN_HANDLERS_H
#define WIN_HANDLERS_H

#include <windows.h>

extern AppState app_state; // APP STATE FROM MAIN.C

void on_paint(HWND hwnd);
void on_mousemove(LPARAM lparam);
void on_leftmouseup(LPARAM lparam);
void on_rightmousedown(LPARAM lparam);
void on_rightmouseup(LPARAM lparam);


#endif