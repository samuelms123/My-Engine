#ifndef TYPES_H
#define TYPES_H

#include "mathmy.h"
#include <stdbool.h>

typedef struct {
    float size;
    Vec2 velocity;
    Vec2 position;
    bool is_dropped;
    float bounce_force;
} Entity;

typedef struct {
    float gravity;
    float floor;
    int rect_count;
    int dropped_count;
    Entity* grabbed_rect;
    Entity* rects;
} EngineState;

typedef struct {
    int window_w;
    int window_h;
    Vec2 mouse_pos;
    EngineState physics;
} AppState;

#endif