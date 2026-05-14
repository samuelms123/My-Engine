#ifndef MY_MATH_H
#define MY_MATH_H

#define MY_PI 3.14159265359f


typedef struct myVec2 {
    float x;
    float y;
} myVec2;

typedef struct myRot {
    float cos;
    float sin;
} myRot;

typedef struct myTransform {
    myVec2 p; // position
    myRot q; // quaternion

} myTransform;

myVec2 myMath_Add(myVec2 a, myVec2 b);
myVec2 myMath_Sub(myVec2 a, myVec2 b);
myVec2 myMath_Scale(myVec2 v, float scalar);
myVec2 myMath_Norm(myVec2 v);
myVec2 myMath_Inverse(myVec2 v);
myVec2 myMath_Transform(myVec2 v, myTransform transform);
float myMath_Distance(myVec2 a, myVec2 b);
float myMath_SquaredDistance(myVec2 a, myVec2 b);
float myMath_SquaredLength(myVec2 v);
float myMath_Dot(myVec2 a, myVec2 b);
float myMath_Abs(float a);
float myMath_Clamp(float value, float min, float max);
float myMath_Min(float a, float b);
float myMath_Max(float a, float b);
float myMath_Lerp(float a, float b, float t);
float myMath_InvLerp(float a, float b, float v);


#endif