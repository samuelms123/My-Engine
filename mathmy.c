#include "mathmy.h"
#include <math.h>

Vec2 mathmy_vec2_add(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vec2 mathmy_vec2_sub(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Vec2 mathmy_vec2_scale(Vec2 v, float scalar) {
    Vec2 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

float mathmy_vec2_squared_lenght(Vec2 v) {
    return (v.x * v.x) + (v.y * v.y);
}

Vec2 mathmy_vec2_norm(Vec2 v) {
    float length = sqrtf(mathmy_vec2_squared_lenght(v));

    if (length > 0) {
        return (Vec2){ v.x / length, v.y / length };
    }
}

float mathmy_vec2_dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float  mathmy_abs(float a) {
    if (a < 0) {
        return a * -1;
    }
    return a;
}

float mathmy_pow(float base, float power) {
    return base * power;
}