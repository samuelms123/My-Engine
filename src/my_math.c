#include "../include/my_math.h"
#include <math.h>

myVec2 myMath_Add(myVec2 a, myVec2 b) {
    myVec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

myVec2 myMath_Sub(myVec2 a, myVec2 b) {
    myVec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

myVec2 myMath_Scale(myVec2 v, float scalar) {
    myVec2 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    return result;
}

myVec2 myMath_Inverse(myVec2 v) {
    v.x = v.x * -1;
    v.y = v.y * -1;
    return v;
}

myVec2 myMath_Transform(myVec2 v, myTransform transform) {
    // rotate
    float x2 = (transform.q.cos * v.x) - (transform.q.sin * v.y);
    float y2 = (transform.q.sin * v.x) + (transform.q.cos * v.y);
    
    // translate
    x2 += transform.p.x;
    y2 += transform.p.y;

    return (myVec2){x2, y2};
}

myVec2 myMath_Norm(myVec2 v) {
    float length = sqrtf(myMath_SquaredLength(v));

    if (length > 0) {
        return (myVec2){ v.x / length, v.y / length };
    }
    return (myVec2){ 0, 0 };
}

float myMath_SquaredLength(myVec2 v) {
    return (v.x * v.x) + (v.y * v.y);
}

float myMath_Dot(myVec2 a, myVec2 b) {
    return a.x * b.x + a.y * b.y;
}

float  myMath_Abs(float a) {
    if (a < 0) {
        return a * -1;
    }
    return a;
}

float myMath_Distance(myVec2 a, myVec2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return sqrtf(dx * dx + dy * dy);
}

float myMath_SquaredDistance(myVec2 a, myVec2 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;

    return (dx * dx) + (dy * dy);
}

float myMath_Clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    else {
        return value;
    }
}

float myMath_Min(float a, float b) {
    if (b < a ) {
        return b;
    }
    return a;
}
float myMath_Max(float a, float b) {
    if (b > a ) {
        return b;
    }
    return a;
}

float myMath_Lerp(float a, float b, float t) {
    return (1.0f - t) * a + b * t;
}

float myMath_InvLerp(float a, float b, float v) {
    return (v - a) / ( b - a );
}