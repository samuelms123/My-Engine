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
    state->grabbed_rect_offset = (Vec2){0.0f, 0.0f};
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

        if (state->grabbed_rect == rect) {
            rect->is_dropped = false;
            rect->velocity.y = 0;
            continue;
        }

        // Apply gravity only if rect is not grabbed
        engine_apply_gravity(rect, state->gravity, dt);

        bool hit_surface = false;
        float surface_y = state->floor;

        // Check RECT i collision with RECT j
        for (int j = 0; j < state->dropped_count; j++) {
            if (j == i) continue;
            if(engine_is_entity_colliding(rect, &state->rects[j])) {
                hit_surface = true;
                surface_y = state->rects[j].position.y;
                break;
            }        
        }
        
        // Check floor coll if not collided yet
        if (!hit_surface && (rect->position.y + rect->size > state->floor)) {
            hit_surface = true;
            surface_y = state->floor;
        }

        if (hit_surface) {
            engine_apply_bounce(rect, surface_y);

            // Stop when bouncing small enough
            if (mathmy_abs(rect->velocity.y) < 80.0f) {
                rect->velocity.y = 0;
                rect->is_dropped = false;
            }
        }
        
    }
}

void engine_render(HWND hwnd) {
        InvalidateRect(hwnd, NULL, TRUE); 
        UpdateWindow(hwnd);
}

bool engine_is_entity_colliding(Entity* i, Entity* j) {
    return (
        (i->position.y + i->size > j->position.y) &&
        (i->position.y < j->position.y + j->size) &&
        (i->position.x + i->size > j->position.x) &&
        (i->position.x < j->position.x + j->size)
    );
}

void engine_apply_gravity(Entity* rect, float gravity, float dt) {
    rect->velocity.y += gravity * dt;
    rect->position.y += rect->velocity.y * dt;
}

void engine_apply_bounce(Entity* rect, float bounce_surface_lvl) {
    rect->position.y = bounce_surface_lvl - rect->size;
    rect->velocity.y = -rect->velocity.y * rect->bounce_force;
}
