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
void engine_apply_bounce(Entity* rect, float bounce_surface_lvl);
bool engine_is_entity_on_surface(Entity* rect, float surface_y);

#endif