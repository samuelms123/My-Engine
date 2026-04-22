#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>
#include "common.h"

void engine_init(EngineState* state);
void engine_update(EngineState* state, float dt);
void engine_render(HWND hwnd);

#endif