#ifndef MATHMY_H
#define MATHMY_H


typedef struct {
    float x;
    float y;
} Vec2;

Vec2 mathmy_vec2_add(Vec2 a, Vec2 b);
float  mathmy_abs(float a);

#endif