#include "engine.h"
#include "common.h"
#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void engine_init(EngineState* state) {
    state->floor = 960.0f;
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

        state->rects[i].size = 100.0f;
        state->rects[i].mass = 0.0005f;
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
        bool hit_moving_rect = false;
        float surface_y = state->floor;

        // Check RECT i collision with RECT j
        for (int j = 0; j < state->dropped_count; j++) {
            if (j == i) continue;
            if(engine_is_entity_colliding(rect, &state->rects[j])) {
                hit_surface = true;
                Entity* collided_rect = &state->rects[j];

                if (collided_rect->velocity.y != 0) {
                    hit_moving_rect = true;
                    engine_apply_1d_elastic_collision(rect, collided_rect);
                }

                surface_y = state->rects[j].position.y;
                break;
            }        
        }
        
        // Check floor coll if not collided yet
        if (!hit_surface && (rect->position.y + rect->size > state->floor)) {
            hit_surface = true;
            surface_y = state->floor;
        }

        if (hit_surface && !hit_moving_rect) {
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

void engine_apply_1d_elastic_collision(Entity* rect1, Entity* rect2) {
    float v1 = rect1->velocity.y;
    float v2 = rect2->velocity.y;
    float m1 = rect1->mass;
    float m2 = rect2->mass;
    float total_mass = m1+m2;

    if (m1 == m2) {
        rect1->velocity.y = (2.0f * m2 * v2) / total_mass;
        rect2->velocity.y = (2.0f * m1 * v1) / total_mass;
    }
    else {
        rect1->velocity.y = ((m1 - m2) * v1 + (2.0f * m2 * v2)) / total_mass;
        rect2->velocity.y = ((m2 - m1) * v2 + (2.0f * m1 * v1)) / total_mass;
    }
}
