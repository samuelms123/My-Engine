#include "engine.h"
#include "common.h"
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void engine_init(EngineState* state) {
    state->floor = 600.0f;
    state->gravity = 1150.0f;
    state->rect = malloc(sizeof(Entity));

    if (state->rect == NULL) {
            printf("rect is null");
            return;
        }

    state->rect->size = 100;
    state->rect->velocity = (Vec2){0.0f, 0.0f};
    state->rect->position = (Vec2){0.0f, 0.0f};
    state->rect->is_dropped = false;
    state->rect->bounce_force = 0.55f;
}

void engine_update(EngineState* state, float dt) {
    Entity* rect = state->rect;

    if (!rect) return;

    if (rect->is_dropped) {
        // Apply gravity
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
int engine_render(HWND hwnd) {
    return 0;
}
