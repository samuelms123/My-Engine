#ifndef MYNGN_MATH_H
#define MYNGN_MATH_H

#define MY_PI 3.14159265359f


typedef struct {
    float x;
    float y;
} myVec2;

myVec2 my_Math_Add(myVec2 a, myVec2 b);
myVec2 my_Math_Sub(myVec2 a, myVec2 b);
myVec2 my_Math_Scale(myVec2 v, float scalar);
myVec2 my_Math_Norm(myVec2 v);
myVec2 my_Math_Inverse(myVec2 v);
float my_Math_Distance(myVec2 a, myVec2 b);
float my_Math_SquaredLength(myVec2 v);
float my_Math_Dot(myVec2 a, myVec2 b);
float my_Math_Abs(float a);


#endif