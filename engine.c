#include "engine.h"
#include "common.h"
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void engine_init(EngineState* state) {
    state->floor = 560.0f;
    state->gravity = 1150.0f;
    state->rect_count = 5;
    state->dropped_count = 0;
    state->grabbed_rect = NULL;
    state->rects = malloc(sizeof(Entity) * state->rect_count);

    if (state->rects == NULL) {
            printf("rects is null");
            return;
        }

    for (int i = 0; i < state->rect_count; i++) {

        state->rects[i].size = 100;
        state->rects[i].velocity = (Vec2){0.0f, 0.0f};
        state->rects[i].position = (Vec2){0.0f, 0.0f};
        state->rects[i].is_dropped = false;
        state->rects[i].bounce_force = 0.55f;
    }
}

void engine_update(EngineState* state, float dt) {
    if (!state->rects) return;

    for (int i = 0; i < state->rect_count; i++) {
        Entity* rect = &state->rects[i];
        if (rect->is_dropped) {

            if (state->grabbed_rect == rect) {
                rect->is_dropped = false;
                rect->velocity.y = 0;
                continue;
            }
            // Apply gravity only if rect is not grabbed
            rect->velocity.y += state->gravity * dt;
            rect->position.y += rect->velocity.y * dt;

            // Check collision
            if (rect->position.y + rect->size > state->floor) {
                rect->position.y = state->floor - rect->size;
                rect->velocity.y = -rect->velocity.y * rect->bounce_force;
                
                // Stop when bouncing small enough
                if (mathmy_abs(rect->velocity.y) < 80.0f) {
                    rect->velocity.y = 0;
                    rect->is_dropped = false;
                }
            }
        }
    }
}

void engine_render(HWND hwnd) {
        InvalidateRect(hwnd, NULL, TRUE); 
        UpdateWindow(hwnd);
}
