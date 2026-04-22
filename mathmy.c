#include "mathmy.h"

Vec2 mathmy_vec2_add(Vec2 a, Vec2 b) {
    Vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}
float  mathmy_abs(float a) {
    if (a < 0) {
        return a * -1;
    }
    return a;
}