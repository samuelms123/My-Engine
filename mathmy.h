#ifndef MATHMY_H
#define MATHMY_H


typedef struct {
    float x;
    float y;
} Vec2;

Vec2 mathmy_vec2_add(Vec2 a, Vec2 b);
Vec2 mathmy_vec2_sub(Vec2 a, Vec2 b);
Vec2 mathmy_vec2_scale(Vec2 v, float scalar);
Vec2 mathmy_vec2_norm(Vec2 v);
float mathmy_vec2_squared_length(Vec2 v);
float mathmy_vec2_dot(Vec2 a, Vec2 b);
float mathmy_abs(float a);
float mathmy_pow(float base, float power);


#endif