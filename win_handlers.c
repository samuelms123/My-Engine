#include <windows.h>
#include <windowsx.h>
#include "engine.h"    
#include "mathmy.h"       
#include "win_handlers.h"

void on_paint(HWND hwnd) {
    PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            if (app_state.physics.dropped_count < app_state.physics.rect_count) {
                Rectangle(hdc, (int)app_state.mouse_pos.x, (int)app_state.mouse_pos.y, (int)app_state.mouse_pos.x + 100, (int)app_state.mouse_pos.y + 100);
            }
            else {
                DrawText(hdc, L"Out of rectangles:(", -1, &ps.rcPaint, DT_CENTER);
            }

            for (int i = 0; i < app_state.physics.dropped_count; i++) {
                Entity* r = &app_state.physics.rects[i];
                Rectangle(hdc, (int)r->position.x, (int)r->position.y, (int)r->position.x + r->size, (int)r->position.y + r->size);
            }

            EndPaint(hwnd, &ps);
}


void on_mousemove(LPARAM lp) {
    app_state.mouse_pos.x = (float)GET_X_LPARAM(lp);
    app_state.mouse_pos.y = (float)GET_Y_LPARAM(lp);
}


void on_leftmouseup(LPARAM lparam) {
    EngineState* es = &app_state.physics;
    if (es->dropped_count < es->rect_count) {
        int next_ent_index = es->dropped_count;
        es->dropped_count += 1;

        es->rects[next_ent_index].position = (Vec2){app_state.mouse_pos.x, app_state.mouse_pos.y};
        es->rects[next_ent_index].is_dropped = true;
    }
}
/*
void on_rightmousedown(LPARAM lparam) {
    
}
void on_rightmouseup(LPARAM lparam);
*/