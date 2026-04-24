#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include "common.h"
#include <stdbool.h>

void engine_init(EngineState* state);
void engine_update(EngineState* state, float dt);
void engine_render(HWND hwnd);
bool engine_is_entity_colliding(Entity* a, Entity* b);
void engine_apply_gravity(Entity* rect, float gravity, float dt);
void engine_apply_1d_floor_elastic_collision(Entity* rect, float bounce_surface_lvl);
void engine_apply_1d_elastic_collision(Entity* rect1, Entity* rect2);
enum collision_side engine_resolve_collision_side(Entity* rect, Entity* collided_rect);

#endif