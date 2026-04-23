#include <windows.h>
#include <windowsx.h>
#include "engine.h"    
#include "mathmy.h"       
#include "win_handlers.h"
#include <stdbool.h>

void on_paint(HWND hwnd) {
    PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            if (app_state.physics.dropped_count < app_state.physics.rect_count) {
                DrawText(hdc, L"Click left mouse to drop a rect!:)", -1, &ps.rcPaint, DT_CENTER);
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

    Entity* grabbed_rect = app_state.physics.grabbed_rect;

    if (grabbed_rect) {
        grabbed_rect->position.x = app_state.mouse_pos.x;
        grabbed_rect->position.y = app_state.mouse_pos.y;
    }
}


void on_leftmouseup(LPARAM lp) {
    EngineState* es = &app_state.physics;
    if (es->dropped_count < es->rect_count) {
        int next_ent_index = es->dropped_count;
        es->dropped_count += 1;

        es->rects[next_ent_index].position = (Vec2){app_state.mouse_pos.x, app_state.mouse_pos.y};
        es->rects[next_ent_index].is_dropped = true;
    }
}

void on_rightmousedown(LPARAM lparam) {
    Vec2 mouse_pos = app_state.mouse_pos;

    for (int i = 0; i < app_state.physics.dropped_count; i++) {
        Entity* rect = &app_state.physics.rects[i];

        bool inside_y = (rect->position.y < mouse_pos.y) && (rect->position.y + rect->size > mouse_pos.y);
        bool inside_x = (rect->position.x < mouse_pos.x) && (rect->position.x + rect->size > mouse_pos.x);

        if (inside_x && inside_y) {
            app_state.physics.grabbed_rect = rect;
            break;
        }
    }
    
}

void on_rightmouseup(LPARAM lparam) {
    Entity* grabbed_rect = app_state.physics.grabbed_rect;
    if (grabbed_rect == NULL) return;
    
    grabbed_rect->is_dropped = true;
    app_state.physics.grabbed_rect = NULL;
}
